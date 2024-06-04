// Copyright(c) Microsoft Corporation. All rights reserved.
#pragma once
#if !defined(__cplusplus)
    #error C++11 required
#endif
#include <XUser.h>
#include <XGameSave.h>
#include <XGameErr.h>
#include <XSystem.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <future>

// provide impl of this in the consuming code
namespace XgsUtil{
    HRESULT ReportFailure(HRESULT hrResult, const char* msg);
}

#undef RETURN_IF_FAILED
#undef RETURN_IF_NULL_ALLOC
#undef RETURN_HR_IF

#define RETURN_IF_FAILED(hr) do {HRESULT __hr = (hr); if (FAILED(__hr)){return XgsUtil::ReportFailure(__hr, #hr); }} while(0,0)
#define RETURN_IF_NULL_ALLOC(ptr) do{ if ((ptr) == nullptr){return XgsUtil::ReportFailure(E_OUTOFMEMORY, #ptr); }} while(0,0)
#define RETURN_HR_IF(hr, test) do {if ((test)){return XgsUtil::ReportFailure((hr), #test);}} while(0,0)

#ifndef REPORTFAILURE_DEFINED // provide a simple impl of ReportFailure
namespace XgsUtil{
    HRESULT ReportFailure(HRESULT hrResult, const char* msg)
    {
        wchar_t buffer[1024];
        _snwprintf_s(buffer, _countof(buffer), _TRUNCATE, L"Error hr=0x%08x msg=\'%S\'\n", hrResult, msg);
        OutputDebugStringW(buffer);
        return hrResult;
    }
}
#endif


// Common C++11 types for XGameSave C++11 wrappers

namespace Microsoft {
    namespace Xbox {
        namespace Wrappers {
            namespace Details
            {
            
                // Worker
                typedef std::function<void()> WorkItem;
                
                class WorkItemThread
                {
                public:
                    WorkItemThread() = default;

                    ~WorkItemThread()
                    {
                        if (m_worker.joinable())
                        {
                            m_working = false;
                            m_worker.join();
                        }
                    }
                    
                    void QueueWorkItem(WorkItem wi) {
                        std::unique_lock<std::mutex> lock{ m_lock };
                        m_items.push_back(std::move(wi));
                        StartThread();
                    }

                protected:
                    void StartThread()
                    {
                        if (!m_worker.joinable())
                        {
                            m_working = true;
                            m_worker = std::move(std::thread(&WorkItemThread::Worker, this));
                        }
                    }
                    void Worker()
                    {
                        while(m_working)
                        {
                            std::list<WorkItem> items;
                            try
                            {
                                {
                                    std::unique_lock<std::mutex> lock{ m_lock };
                                    if (!m_items.empty())
                                    {
                                        std::swap(items, m_items);
                                    }
                                }
                            }
                            catch(...)
                            {
                                m_working = false;
                                return;
                                
                            }
                            for(auto it = items.begin(); it != items.end();it++)
                            {
                                (*it)();
                            }
                            // Sleep in a waitable state to allow APC on this thread
                            SleepEx(0, TRUE);
                        }
                    }


                private:
                    std::mutex m_lock;
                    std::atomic<bool> m_working = false;
                    std::thread m_worker;
                    std::list<WorkItem> m_items;
                };

                template <typename R>
                bool is_ready(std::future<R> const &f)
                {
                    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
                }
                
                
                #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_GAMES)
                struct INFER_WINDOW_HWND_CONTEXT
                {
                    DWORD   processId;
                    HWND    hwnd;
                    bool    includeMinimizedWindows;
                };

                static BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
                {
                    auto *context = reinterpret_cast<INFER_WINDOW_HWND_CONTEXT *>(lParam);
                    HWND              hwndOwner;
                    DWORD             exStyle;
                    WINDOWPLACEMENT wp;

                    hwndOwner = GetWindow(hwnd, GW_OWNER);
                    exStyle = static_cast<DWORD>(GetWindowLong(hwnd, GWL_EXSTYLE));
                    GetWindowPlacement(hwnd, &wp);

                    //
                    //  Skip over the windows we don't care about:
                    //
                    //  1) Child windows (unless they specify the WS_EX_APPWINDOW flag)
                    //  2) Windows that aren't visible
                    //  3) Tooltips
                    //  4) Minimized if includeMinimizedWindows flag is set
                    //
                    //
                    // Taskmgr / Tray / Alt-Tab have similar code to exclude these windows
                    //
                    if ((hwndOwner && IsWindowVisible(hwndOwner) && !(exStyle & WS_EX_APPWINDOW)) ||
                        !IsWindowVisible(hwnd) ||
                        (!context->includeMinimizedWindows && wp.showCmd == SW_SHOWMINIMIZED) ||
                        (exStyle & WS_EX_TOOLWINDOW))
                    {
                        return TRUE;
                    }

                    //
                    //  If the process for the window doesn't match the process we're looking
                    //  for, skip it.
                    //
                    DWORD windowProcessId = 0;
                    if (::GetWindowThreadProcessId(hwnd, &windowProcessId) == 0 ||
                        context->processId != windowProcessId)
                    {
                        return TRUE;
                    }

                    //
                    //  We're done - get the window hwnd.
                    //
                    context->hwnd = hwnd;

                    // We've found our hwnd, we're done so stop enumerating.
                    return FALSE;
                }
                
                static HRESULT GetGameWindow(_Out_ HWND* windowHandle)
                {
                    auto currentProcessId = ::GetCurrentProcessId();

                    // Get the game window by:
                    // 1 - Getting the foreground window if it matches the process ID
                    // 2 - Else getting the first visible window owned by the process.
                    // 3 - Else try to fall back to a minimized window owned by the process.
                    // 4 - Else return failure

                    // 1 - Getting the foreground window if it matches the process ID
                    auto possibleWindowHandle = ::GetForegroundWindow();
                    if (possibleWindowHandle != nullptr)
                    {
                        DWORD windowProcessId = 0;
                        ::GetWindowThreadProcessId(possibleWindowHandle, &windowProcessId);
                        if (currentProcessId != windowProcessId)
                        {
                            possibleWindowHandle = nullptr;
                        }
                    }

                    if (possibleWindowHandle == nullptr)
                    {
                        INFER_WINDOW_HWND_CONTEXT context = { 0 };
                        context.processId = currentProcessId;
                        context.hwnd = nullptr;

                        // 2 - Else getting the first visible window owned by the process.
                        context.includeMinimizedWindows = false;
                        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&context));

                        if (context.hwnd == nullptr)
                        {
                            // 3 - Else try to fall back to a minimized window owned by the process.
                            context.includeMinimizedWindows = true;
                            EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&context));
                        }

                        possibleWindowHandle = context.hwnd;
                    }

                    *windowHandle = possibleWindowHandle;

                    // 4 - Else return failure
                    return (possibleWindowHandle != nullptr ? S_OK : E_GAMERUNTIME_WINDOW_NOT_FOREGROUND);
                    
                }
                
                static bool is_uithread()
                {
                    XSystemDeviceType type = XSystemGetDeviceType();
                    if (type == XSystemDeviceType::Pc)
                    {
                        HWND hwnd;
                        if (SUCCEEDED(GetGameWindow(&hwnd) && hwnd != (HWND)1))
                        {
                            DWORD thisTid = GetCurrentThreadId();
                            DWORD windowTid, windowPid;
                            windowTid = ::GetWindowThreadProcessId(hwnd, &windowPid);
                            if (windowPid == GetCurrentProcessId()) // only bother checking if the proper hwnd was detected
                            {
                                return thisTid == windowTid;
                            }
                        }
                    }
                    return false;
                }
                #else //DISABLE_UITHREAD_CHECK
                __forceinline bool is_uithread()
                {
                    return false;
                }
                #endif

            } // namespace  Details
            
            namespace GameSave {
                
                struct ContainerInfo;
                struct BlobInfo;
                
                typedef std::vector<uint8_t> BlobData;
                typedef std::map<std::string, BlobData> BlobDataMap;
                typedef std::vector<std::string> BlobNames;
                typedef std::vector<std::string> ContainerNames;
                typedef std::vector<BlobInfo> BlobInfoSet;
                typedef std::vector<ContainerInfo> ContainerInfoSet;
                
                // tuples
                typedef std::tuple<HRESULT, int64_t> QuotaResult;
                typedef std::tuple<HRESULT, ContainerNames> QueryContainersResult;
                typedef std::tuple<HRESULT, BlobNames> QueryBlobNamesResult;
                typedef std::tuple<HRESULT, ContainerInfoSet> ContainerInfoResult;
                typedef std::tuple<HRESULT, BlobData> BlobResult;
                typedef std::tuple<HRESULT, BlobInfoSet> BlobInfoResult;
                typedef std::tuple<HRESULT, BlobDataMap> BlobDataResult;
                
                struct BlobInfo
                {
                    BlobInfo(const XGameSaveBlobInfo* info)
                    {
                        if (info->name && info->name[0] != '\0')
                        {
                            name = info->name;
                        }
                        size = info->size;
                    }
                    std::string name;
                    uint32_t size;
                };
                
                struct ContainerInfo
                {
                    ContainerInfo(const XGameSaveContainerInfo* info)
                    {
                        if (info->name && info->name[0] != '\0')
                        {
                            name = info->name;
                        }
                        if (info->displayName && info->displayName[0] != '\0')
                        {
                            display = info->displayName;
                        }
                        blobCount = info->blobCount;
                        totalSize = info->totalSize;
                        lastModifiedTime = info->lastModifiedTime;
                        needsSync = info->needsSync;
                    }

                    std::string name;
                    std::string display;
                    uint64_t totalSize;
                    time_t lastModifiedTime;
                    bool needsSync;
                    uint32_t blobCount;
                };
                
                
                namespace Details
                {
                    // traits
                    struct XGameSaveDefaultTraits
                    {
                        inline static bool IsUsingSyncOnDemand()
                        {
                            return false;
                        }
                        inline static bool IsUsingAsyncQueue()
                        {
                            return true;
                        }
                        inline static bool IsUsingThrowOnUiThread()
                        {
                            return true;
                        }
                    };

                    struct XGameSaveSyncOnDemandTraits : public XGameSaveDefaultTraits
                    {
                        inline static bool IsUsingSyncOnDemand()
                        {
                            return true;
                        }
                    };

                    // raii types for managing data
                    struct xuser_closer { void operator()(XUserHandle h) { if (h) { XUserCloseHandle(h); } } };
                    typedef std::unique_ptr<XUser, xuser_closer> unique_user_handle;

                    struct xtaskqueue_closer { void operator()(XTaskQueueHandle h) { if (h) { XTaskQueueCloseHandle(h); } } };
                    typedef std::unique_ptr<XTaskQueueObject, xtaskqueue_closer> unique_taskqueue_handle;

                    struct gamesave_provider_closer { void operator()(XGameSaveProviderHandle h) { if (h) { XGameSaveCloseProvider(h); } } };
                    typedef std::unique_ptr<XGameSaveProvider, gamesave_provider_closer> unique_gsprovider_handle;

                    struct gamesave_container_closer { void operator()(XGameSaveContainerHandle h) { if (h) { XGameSaveCloseContainer(h); } } };
                    typedef std::unique_ptr<XGameSaveContainer, gamesave_container_closer> unique_gscontainer_handle;

                    struct gamesave_update_closer { void operator()(XGameSaveUpdateHandle h) { if (h) { XGameSaveCloseUpdate(h); } } };
                    typedef std::unique_ptr<XGameSaveUpdate, gamesave_update_closer> unique_gsupdate_handle;
                    
                    namespace Mxwd = Microsoft::Xbox::Wrappers::Details;
                    
                    // Base wrapper with different traits to enable different usage modes
                    template<typename TRuntimeTraits>
                    class XGameSave final
                    {
                    public:
                        typedef TRuntimeTraits RuntimeTraits;
                        typedef XGameSave<TRuntimeTraits> RuntimeType;
                        
                        XGameSave() {}
                        XGameSave(XGameSave&& other) {}
                        XGameSave(XGameSave&) = delete;
                        ~XGameSave(){ }
                        
                        //operators
                        inline bool operator ==(const XGameSave& rhs) = delete;
                        inline bool operator !=(const XGameSave& rhs) = delete;
                        
                        std::future<HRESULT> Initialize(XUserHandle userHandle, const std::string& serviceConfigurationId)
                        {
                            return P_Initialize(userHandle, serviceConfigurationId, true);
                        }
                        
                        std::future<QuotaResult>  GetQuota()
                        {
                            return P_GetQuota(true);
                        }
                        
                        std::future<QueryContainersResult> QueryContainers(const std::string& containerPrefix)
                        {
                            return P_QueryContainers(containerPrefix, true);
                        }

                        std::future<QueryContainersResult> QueryContainers()
                        {
                            std::string empty;
                            return QueryContainers(empty);
                        }
                        
                        std::future<QueryBlobNamesResult> QueryBlobs(const std::string& container)
                        {
                            return P_QueryBlobs(container, true);
                        }

                        std::future<ContainerInfoResult> QueryContainer(const std::string& container)
                        {
                            return P_QueryContainer(container, true);
                        }
                        
                       
                        std::future<BlobInfoResult> QueryBlobInfo(const std::string& container)
                        {
                            return P_QueryBlobInfo(container, false);
                        }

                        std::future<HRESULT> DeleteContainer(const std::string& container)
                        {
                            return P_DeleteContainer(container, true);
                        }

                        std::future<BlobResult> LoadData(const std::string& container, const std::string& blobName )
                        {
                            // NOTE: using an async here as the actual LoadData will be 
                            // serialized against the WorkItemQueue.  If the queue is used here
                            // as well it would deadlock as it only processes one thing at a time
                            return std::async([this, container, blobName]() -> BlobResult 
                            {
                                BlobNames names;
                                names.push_back(blobName);
                                auto bdr = P_LoadData(container, names, false).get();
                                HRESULT hr = std::get<0>(bdr);
                                auto blobMap = std::get<1>(bdr);
                                if (SUCCEEDED(hr))
                                {
                                    return std::make_tuple(hr, std::move(blobMap[blobName]));
                                }
                                BlobData empty;
                                return std::make_tuple(hr, empty);
                            });
                        }
                        
                        std::future<BlobDataResult> LoadData(const std::string& containerName, const BlobNames& blobNames )
                        {
                            return P_LoadData(containerName, blobNames, true);
                        }
                        
                        std::future<HRESULT> SaveData(const std::string& container, const BlobDataMap& dataMap)
                        {
                            BlobNames empty;
                            return SaveData(container, dataMap, empty);
                        }
                        std::future<HRESULT> SaveData(const std::string& containerName, const BlobDataMap& dataMap, const BlobNames& toDelete)
                        {
                            return P_SaveData(containerName, dataMap, toDelete, true);
                        }

                    protected:
                    
                        template<typename TData>
                        bool P_SubmitTupleWork(std::shared_ptr<std::promise<std::tuple<HRESULT, TData>>>& promise, Mxwd::WorkItem wi, bool queueWorker)
                        {
                            if (m_providerHandle.get() != nullptr)
                            {
                                try
                                {
                                    if (queueWorker)
                                    {
                                        m_workItems.QueueWorkItem(wi);
                                    }
                                    else
                                    {
                                        wi();
                                    }
                                    return true;
                                }
                                catch(...)
                                {
                                    promise->set_exception(std::current_exception());
                                }
                            }
                            else
                            {
                                TData empty;
                                promise->set_value(std::make_tuple(E_INVALIDARG,empty));
                            }
                            return false;
                        }
                        
                        bool P_SubmitWork(std::shared_ptr<std::promise<HRESULT>>& promise, Mxwd::WorkItem wi, bool queueWorker, bool checkProvider = true)
                        {
                            if (!checkProvider || (checkProvider && m_providerHandle.get() != nullptr))
                            {
                                try
                                {
                                    if (queueWorker)
                                    {
                                        m_workItems.QueueWorkItem(wi);
                                    }
                                    else
                                    {
                                        wi();
                                    }
                                    return true;
                                }
                                catch(...)
                                {
                                    promise->set_exception(std::current_exception());
                                }
                            }
                            else
                            {
                                promise->set_value(E_INVALIDARG);
                            }
                            return false;
                        }
                        
                        template<typename TData>
                        std::shared_ptr<std::promise<std::tuple<HRESULT, TData>>> P_CreateTuplePromise(
                            std::function<HRESULT()> initializeCheck, bool& initSuccess)
                        {
                            initSuccess = false;
                            try
                            {
                                auto resultPromise = std::make_shared<std::promise<std::tuple<HRESULT, TData>>>();
                                auto initHr = initializeCheck();
                                if (FAILED(initHr))
                                {
                                    TData empty;
                                    resultPromise->set_value(std::make_tuple(initHr, empty));
                                }
                                else
                                {
                                    initSuccess = true;
                                }
                                return resultPromise;
                            }
                            catch(...)
                            {
                                return nullptr;
                            }
                        }
                        
                        std::shared_ptr<std::promise<HRESULT>> P_CreatePromise(std::function<HRESULT()> initializeCheck, bool& initSuccess)
                        {
                            initSuccess = false;
                            auto resultPromise = std::make_shared<std::promise<HRESULT>>();
                            auto initHr = initializeCheck();
                            if (FAILED(initHr))
                            {
                                resultPromise->set_value(initHr);
                            }
                            else
                            {
                                initSuccess = true;
                            }
                            return resultPromise;
                        }
                        
                        std::future<HRESULT> P_Initialize(XUserHandle userHandle, const std::string& serviceConfigurationId, bool queueWorker)
                        {
                            bool initSuccess;
                            std::unique_ptr<XAsyncBlock> asyncBlock;

                            // kickoff in scope to validate inputs
                            auto initHr = [&]() -> HRESULT
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() != nullptr);
                                XUserHandle userCopy = nullptr;
                                RETURN_IF_FAILED(XUserDuplicateHandle(userHandle, &userCopy));
                                m_userHandle.reset(userCopy);
                                asyncBlock = std::make_unique<XAsyncBlock>();
                                RETURN_IF_NULL_ALLOC(asyncBlock);
                                RETURN_IF_FAILED(XTaskQueueCreate(
                                    XTaskQueueDispatchMode::ThreadPool,
                                    XTaskQueueDispatchMode::ThreadPool,
                                    &m_queue));
                                asyncBlock->queue = m_queue;

                                RETURN_IF_FAILED(XGameSaveInitializeProviderAsync(
                                    m_userHandle.get(), 
                                    serviceConfigurationId.c_str(), 
                                    RuntimeTraits::IsUsingSyncOnDemand(), 
                                    asyncBlock.get() ));
                                m_queueHandle.reset(m_queue);
                                return S_OK;
                            };
                            auto resultPromise = P_CreatePromise(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();
                            
                            // queue up wi to wait on results from XAsyncQueue'ed method
                            auto wi = [this, resultPromise, asyncPtr = asyncBlock.get()]()
                            {
                                std::unique_ptr<XAsyncBlock> asyncCleanup(asyncPtr);
                                XGameSaveProviderHandle provider = nullptr;
                                auto resultHr = [&]() {
                                    if (RuntimeTraits::IsUsingThrowOnUiThread())
                                    {
                                        // similar to the start of the operation we
                                        // want to check to see if the caller is doing
                                        // this wait on the UI Thread
                                        if (Mxwd::is_uithread())
                                        {
                                            throw new std::exception("Waiting for Initialize is not supported on the UI Thread");
                                        }
                                    }                                
                                    RETURN_IF_FAILED(XAsyncGetStatus(asyncPtr, true));
                                    RETURN_IF_FAILED(XGameSaveInitializeProviderResult(asyncPtr, &provider));
                                    return S_OK;
                                }();
                                if (SUCCEEDED(resultHr) || resultHr == E_GS_USER_CANCELED)
                                {
                                    m_providerHandle.reset(provider);
                                }
                                resultPromise->set_value(resultHr);
                            };
                            
                            if (P_SubmitWork(resultPromise, wi, queueWorker, false))
                            {
                                asyncBlock.release();
                            }
                            
                            return resultFuture;
                            
                        }

                        std::future<QuotaResult>  P_GetQuota(bool queueWorker)
                        {
                            std::unique_ptr<XAsyncBlock> asyncBlock;

                            auto initHr = [&]() -> HRESULT
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() == nullptr);
                                if (RuntimeTraits::IsUsingAsyncQueue())
                                {
                                    asyncBlock = std::make_unique<XAsyncBlock>();
                                    RETURN_IF_NULL_ALLOC(asyncBlock);
                                    asyncBlock->queue = m_queue;
                                    RETURN_IF_FAILED(XGameSaveGetRemainingQuotaAsync(m_providerHandle.get(), asyncBlock.get()));
                                }
                                return S_OK;
                            };
                            bool initSuccess;
                            auto resultPromise = P_CreateTuplePromise<int64_t>(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();
                            
                            auto wi = [this, resultPromise, asyncPtr = asyncBlock.get()]()
                            {
                                std::unique_ptr<XAsyncBlock> asyncCleanup(asyncPtr);
                                HRESULT hr = S_OK;
                                int64_t quota = 0;
                                if (!RuntimeTraits::IsUsingAsyncQueue())
                                {
                                    hr = XGameSaveGetRemainingQuota(m_providerHandle.get(), &quota);
                                }
                                else
                                {
                                    hr = XAsyncGetStatus(asyncPtr, true);
                                    if (SUCCEEDED(hr))
                                    {
                                        hr = XGameSaveGetRemainingQuotaResult(asyncPtr, &quota);
                                    }
                                }
                                resultPromise->set_value(std::make_tuple(hr, quota));
                            };
                            
                            if (P_SubmitTupleWork(resultPromise, wi, queueWorker))
                            {
                                asyncBlock.release();
                            }
                            
                            return resultFuture;
                        }


                        QueryContainersResult Impl_QueryContainers(const std::string& containerPrefix)
                        {
                            ContainerNames containers;
                            HRESULT hr = XGameSaveEnumerateContainerInfoByName(m_providerHandle.get(), 
                                containerPrefix.c_str(), 
                                &containers, 
                                [](const XGameSaveContainerInfo* info, void* ctx)
                                {
                                    auto names = reinterpret_cast<ContainerNames*>(ctx);
                                    names->push_back(info->name);
                                    return true;
                                });
                            return std::make_tuple(hr, containers);
                        }
                        std::future<QueryContainersResult> P_QueryContainers(const std::string& containerPrefix, bool queueWorker)
                        {
                            auto resultPromise = std::make_shared<std::promise<QueryContainersResult>>();
                            auto resultFuture = resultPromise->get_future();
                            auto wi = [this, resultPromise, containerPrefix]()
                            {
                                try
                                {
                                    resultPromise->set_value( Impl_QueryContainers(containerPrefix));
                                }
                                catch(...)
                                {
                                    resultPromise->set_exception(std::current_exception());
                                }
                            };
                            (void)P_SubmitTupleWork(resultPromise, wi, queueWorker);

                            return resultFuture;
                        }

                        BlobNames Impl_QueryBlobs(const std::string& container)
                        {
                            BlobNames blobs;
                            unique_gscontainer_handle containerHandle;
                            HRESULT hr = [&]()
                            {
                                XGameSaveContainerHandle containerOutput = nullptr;
                                RETURN_IF_FAILED(XGameSaveCreateContainer(
                                    m_providerHandle.get(), 
                                    container.c_str(), 
                                    &containerOutput));
                                containerHandle.reset(containerOutput);
                                RETURN_IF_FAILED(XGameSaveEnumerateBlobInfo(containerHandle.get(), &blobs, 
                                    [](const XGameSaveBlobInfo* info, void* ctx)
                                    {
                                        auto names = reinterpret_cast<BlobNames*>(ctx);
                                        names->push_back(info->name);
                                        return true;
                                    }));
                            }();
                            
                            return std::make_tuple(hr, std::move(blobs));
                        }
                        std::future<QueryBlobNamesResult> P_QueryBlobs(const std::string& container, bool queueWorker)
                        {
                            auto resultPromise = std::make_shared<std::promise<QueryBlobNamesResult>>();
                            auto resultFuture = resultPromise->get_future();
                            auto wi = [this, resultPromise, container]()
                            {
                                try
                                {
                                    resultPromise->set_value(std::move(Impl_QueryBlobs(container)));
                                }
                                catch(...)
                                {
                                    resultPromise->set_exception(std::current_exception());
                                }
                            };
                            
                            (void)P_SubmitTupleWork(resultPromise, wi, queueWorker);
                            return resultFuture;
                        }
                        
                        ContainerInfoResult Impl_QueryContainer(const std::string& container)
                        {
                            ContainerInfoSet infoSet;
                            HRESULT hr = [&]() -> HRESULT
                            {
                                RETURN_IF_FAILED(XGameSaveGetContainerInfo(m_providerHandle.get(), 
                                    container.c_str(), &infoSet, 
                                    [](const XGameSaveContainerInfo* info, void* ctx)
                                {
                                    auto set = reinterpret_cast<ContainerInfoSet*>(ctx);
                                    set->push_back(std::move(ContainerInfo(info)));
                                    return true;
                                }));
                                return S_OK;
                            }();
                            return std::make_tuple(hr, std::move(infoSet));
                        }
                        
                        std::future<ContainerInfoResult> P_QueryContainer(const std::string& container, bool queueWorker = false)
                        {
                            auto resultPromise = std::make_shared<std::promise<ContainerInfoResult>>();
                            auto resultFuture = resultPromise->get_future();
                            auto wi = [this, resultPromise, container]()
                            {
                                try
                                {
                                    resultPromise->set_value(std::move(Impl_QueryContainer(container)));
                                }
                                catch(...)
                                {
                                    resultPromise->set_exception(std::current_exception());
                                }
                            };
                            
                            (void)P_SubmitTupleWork(resultPromise, wi, queueWorker);
                            return resultFuture;
                        }
                        
                        BlobInfoResult Impl_QueryBlobInfo(const std::string& container)
                        {
                            BlobInfoSet infoSet;
                            unique_gscontainer_handle containerHandle;
                            HRESULT hr = [&]()
                            {
                                XGameSaveContainerHandle containerOutput = nullptr;
                                RETURN_IF_FAILED(XGameSaveCreateContainer(m_providerHandle.get(), 
                                    container.c_str(), &containerOutput));
                                containerHandle.reset(containerOutput);
                                RETURN_IF_FAILED(XGameSaveEnumerateBlobInfo(containerHandle.get(), &infoSet,
                                    [](const XGameSaveBlobInfo* info, void* ctx)
                                    {
                                        auto set = reinterpret_cast<BlobInfoSet*>(ctx);
                                        set->push_back(std::move(BlobInfo(info)));
                                        return true;
                                    }));
                                return S_OK;
                            }();
                            return std::make_tuple(hr, std::move(infoSet));
                        }
                        
                        std::future<BlobInfoResult> P_QueryBlobInfo(const std::string& container, bool queueWorker)
                        {
                            auto initHr = [&]()
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() == nullptr);
                                return S_OK;
                            };
                            bool initSuccess = false;
                            auto resultPromise = P_CreateTuplePromise<BlobInfoSet>(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();

                            auto wi = [this, resultPromise, container]()
                            {
                                try
                                {
                                    resultPromise->set_value(std::move(Impl_QueryBlobInfo(container)));
                                }
                                catch(...)
                                {
                                    resultPromise->set_exception(std::current_exception());
                                }
                            };
                            
                            if (initSuccess)
                            {
                                (void)P_SubmitTupleWork(resultPromise, wi, queueWorker);
                            }
                            return resultFuture;
                        }
                        
                        size_t P_GetRequiredSize(const std::string& containerName, const BlobNames& blobNames)
                        {
                            
                            auto blobInfoOp = P_QueryBlobInfo(containerName, false);
                            size_t bufferSize = 0;
                            auto blobInfo = blobInfoOp.get();
                            BlobDataMap blobDataMap;
                            HRESULT hr = std::get<0>(blobInfo);
                            if (SUCCEEDED(hr))
                            {
                                for(const auto& item : std::get<1>(blobInfo))
                                {
                                    for (const auto& blobName : blobNames)
                                    {
                                        if (item.name == blobName)
                                        {
                                            bufferSize += sizeof(XGameSaveBlob) + item.size + item.name.size() + 1;
                                        }
                                    }
                                }
                            }
                            return bufferSize;
                        }

                        std::future<HRESULT> P_DeleteContainer(const std::string& container, bool queueWorker)
                        {
                            bool initSuccess;
                            std::unique_ptr<XAsyncBlock> asyncBlock;

                            auto initHr = [&]() -> HRESULT
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() == nullptr);
                                if (RuntimeTraits::IsUsingAsyncQueue())
                                {
                                    asyncBlock = std::make_unique<XAsyncBlock>();
                                    RETURN_IF_NULL_ALLOC(asyncBlock);
                                    asyncBlock->queue = m_queue;
                                    RETURN_IF_FAILED(XGameSaveDeleteContainerAsync(m_providerHandle.get(), container.c_str(), asyncBlock.get()));
                                }
                                return S_OK;
                            };

                            auto resultPromise = P_CreatePromise(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();
                            if (initSuccess)
                            {
                                auto wi = [this, container, resultPromise, asyncPtr = asyncBlock.get()]()
                                {
                                    HRESULT hr = E_PENDING;
                                    if (!RuntimeTraits::IsUsingAsyncQueue())
                                    {
                                        hr = XGameSaveDeleteContainer(m_providerHandle.get(), container.c_str());
                                    }
                                    else
                                    {
                                        std::unique_ptr<XAsyncBlock> asyncCleanup(asyncPtr);
                                        hr = XAsyncGetStatus(asyncPtr, true);
                                        if (SUCCEEDED(hr))
                                        {
                                            hr = XGameSaveDeleteContainerResult(asyncPtr);
                                        }
                                    }
                                    
                                    resultPromise->set_value(hr);
                                };
                                
                                if (P_SubmitWork(resultPromise, wi, queueWorker))
                                {
                                    asyncBlock.release();
                                }
                            }
                            
                            return resultFuture;
                        }
                        
                        std::future<BlobDataResult> P_LoadData(const std::string& containerName, const BlobNames& blobNames, bool queueWorker )
                        {
                            bool initSuccess;
                            auto asyncBlock = std::make_unique<XAsyncBlock>();
                            unique_gscontainer_handle containerHandle;
                            auto initHr = [&]() -> HRESULT
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() == nullptr);
                                XGameSaveContainerHandle containerOutput = nullptr;
                                RETURN_IF_FAILED(XGameSaveCreateContainer(m_providerHandle.get(), containerName.c_str(), &containerOutput));
                                containerHandle.reset(containerOutput);
                                if (RuntimeTraits::IsUsingAsyncQueue())
                                {
                                    asyncBlock = std::make_unique<XAsyncBlock>();
                                    RETURN_IF_NULL_ALLOC(asyncBlock);
                                    asyncBlock->queue = m_queue;
                                    std::vector<const char*> blobs;
                                    for(const auto& b : blobNames)
                                    {
                                        blobs.push_back(b.c_str());
                                    }
                                    uint32_t count = static_cast<uint32_t>(blobs.size());
                                    
                                    RETURN_IF_FAILED(XGameSaveReadBlobDataAsync(containerHandle.get(), &blobs[0], count, asyncBlock.get()));
                                }
                                return S_OK;
                            };

                            auto resultPromise = P_CreateTuplePromise<BlobDataMap>(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();
                            if(initSuccess)
                            {
                                auto wi = [this, resultPromise, 
                                    asyncPtr = asyncBlock.get(), 
                                    containerPtr = containerHandle.get(),
                                    containerName,
                                    blobNames]()
                                {
                                    unique_gscontainer_handle containerCleanup(containerPtr);
                                    BlobDataMap blobDataMap;
                                    HRESULT hr = E_PENDING;
                                    if (!RuntimeTraits::IsUsingAsyncQueue())
                                    {
                                        auto bufferSize = P_GetRequiredSize(containerName, blobNames);
                                        std::vector<const char*> blobs;
                                        for(const auto& b : blobNames)
                                        {
                                            blobs.push_back(b.c_str());
                                        }
                                        uint32_t count = static_cast<uint32_t>(blobs.size());

                                        BlobData blobDataBuffer(bufferSize);
                                        
                                        XGameSaveBlob* blobData = reinterpret_cast<XGameSaveBlob*>(&blobDataBuffer[0]);
                                        hr =  XGameSaveReadBlobData(containerPtr, &blobs[0], &count, bufferSize, blobData);
                                        if (SUCCEEDED(hr))
                                        {
                                            for (uint32_t i = 0; i < count; i++)
                                            {
                                                blobDataMap.emplace(blobData[i].info.name, std::move(std::vector<uint8_t>(blobData[i].data, blobData[i].data + blobData[i].info.size)));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        std::unique_ptr<XAsyncBlock> asyncCleanup(asyncPtr);

                                        hr = [&]() -> HRESULT
                                        {
                                            RETURN_IF_FAILED(XAsyncGetStatus(asyncPtr, true));
                                            size_t requiredSize;
                                            RETURN_IF_FAILED(XAsyncGetResultSize(asyncPtr, &requiredSize));
                                            BlobData data(requiredSize);
                                            
                                            XGameSaveBlob* blobData = nullptr;
                                            blobData = reinterpret_cast<XGameSaveBlob*>(&data[0]);
                                            uint32_t countOf = 0;
                                            RETURN_IF_FAILED(XGameSaveReadBlobDataResult(asyncPtr, data.size(), blobData, &countOf));
                                            for (uint32_t i = 0; i < countOf; i++)
                                            {
                                                blobDataMap.emplace(blobData[i].info.name, std::move(std::vector<uint8_t>(blobData[i].data, blobData[i].data + blobData[i].info.size)));
                                            }
                                            return S_OK;
                                        }();
                                    }                                
                                    resultPromise->set_value(std::make_tuple(hr, std::move(blobDataMap)));
                                };
                                
                                if (P_SubmitTupleWork(resultPromise, wi, queueWorker))
                                {
                                    asyncBlock.release();
                                    containerHandle.release();
                                }
                            }
                            return resultFuture;
                        }

                        std::future<HRESULT> P_SaveData(const std::string& containerName, const BlobDataMap& dataMap, const BlobNames& toDelete, bool queueWorker)
                        {
                            bool initSuccess;
                            std::unique_ptr<XAsyncBlock> asyncBlock;
                            
                            unique_gscontainer_handle containerHandle;
                            unique_gsupdate_handle updateHandle;
                            auto initHr = [&]() -> HRESULT
                            {
                                RETURN_HR_IF(E_INVALIDARG, m_providerHandle.get() == nullptr);
                                RETURN_HR_IF(E_GS_INVALID_CONTAINER_NAME, containerName.empty());
                                XGameSaveContainerHandle containerOutput = nullptr;
                                RETURN_IF_FAILED(XGameSaveCreateContainer(m_providerHandle.get(), containerName.c_str(), &containerOutput));
                                containerHandle.reset(containerOutput);
                                XGameSaveUpdateHandle updateOutput = nullptr;
                                RETURN_IF_FAILED(XGameSaveCreateUpdate(containerHandle.get(), nullptr, &updateOutput));
                                updateHandle.reset(updateOutput);
                                for(const auto& d : toDelete)
                                {
                                    RETURN_IF_FAILED(XGameSaveSubmitBlobDelete(updateHandle.get(), d.c_str()));
                                }
                                
                                for (const auto& b : dataMap)
                                {
                                    RETURN_IF_FAILED(XGameSaveSubmitBlobWrite(updateHandle.get(), b.first.c_str(), &b.second[0], b.second.size()));
                                }
                                if (RuntimeTraits::IsUsingAsyncQueue())
                                {
                                    asyncBlock = std::make_unique<XAsyncBlock>();
                                    RETURN_IF_NULL_ALLOC(asyncBlock);
                                    asyncBlock->queue = m_queue;
                                    RETURN_IF_FAILED(XGameSaveSubmitUpdateAsync(updateHandle.get(), asyncBlock.get()));
                                }
                                return S_OK;
                            };

                            auto resultPromise = P_CreatePromise(initHr, initSuccess);
                            auto resultFuture = resultPromise->get_future();
                            if (initSuccess)
                            {
                                auto wi = [this, resultPromise, 
                                    asyncPtr = asyncBlock.get(), 
                                    updatePtr = updateHandle.get(), 
                                    containerPtr = containerHandle.get()]()
                                {
                                    std::unique_ptr<XAsyncBlock> asyncCleanup(asyncPtr);
                                    unique_gsupdate_handle updateCleanup(updatePtr);
                                    unique_gscontainer_handle containerCleanup(containerPtr);
                                    
                                    auto finishHr = [&]() -> HRESULT
                                    {
                                        if (!RuntimeTraits::IsUsingAsyncQueue())
                                        {
                                            RETURN_IF_FAILED(XGameSaveSubmitUpdate(updatePtr));
                                        }
                                        else
                                        {
                                            RETURN_IF_FAILED(XAsyncGetStatus(asyncPtr, true));
                                            RETURN_IF_FAILED(XGameSaveSubmitUpdateResult(asyncPtr));
                                        }
                                        return S_OK;
                                    }();
                                    resultPromise->set_value(finishHr);
                                };
                                
                                if (P_SubmitWork(resultPromise, wi, queueWorker))
                                {
                                    asyncBlock.release();
                                    updateHandle.release();
                                    containerHandle.release();
                                }
                            }
                            
                            return resultFuture;
                        }
                        

                    private:
                        unique_user_handle m_userHandle;
                        unique_gsprovider_handle m_providerHandle;
                        unique_taskqueue_handle m_queueHandle;
                        XTaskQueueHandle m_queue;
                        Mxwd::WorkItemThread m_workItems;
                    };

                    // XGameSave wrapper
                    typedef XGameSave<XGameSaveDefaultTraits> XGameSaveProvider;
                    // XGameSave SyncOnDemand (Query/Load may show UX)
                    typedef XGameSave<XGameSaveSyncOnDemandTraits> XGameSaveSyncOnDemandProvider;
                    
                } // namespace Details
                
                class Provider
                {
                public:
                    HRESULT Initialize (XUserHandle userHandle, const std::string& serviceConfigurationId)
                    {
                        auto f = m_provider.Initialize( userHandle, serviceConfigurationId);
                        return f.get();
                    }
                    
                    int64_t GetQuota()
                    {
                        auto f = m_provider.GetQuota();
                        auto r = f.get();
                        return std::get<1>(r);
                    }
                    
                    ContainerNames QueryContainers(const std::string& prefix)
                    {
                        auto f = m_provider.QueryContainers(prefix);
                        auto r = f.get();
                        return std::get<1>(r);
                    }

                    BlobInfoSet QueryContainerBlobs(const std::string& containerName)
                    {
                        auto f = m_provider.QueryBlobInfo(containerName);
                        auto r = f.get();
                        return std::get<1>(r);
                    }

                    BlobData Load( const std::string& containerName, const std::string& blobName )
                    {
                        auto f= m_provider.LoadData(containerName, blobName);
                        auto r = f.get();
                        return std::get<1>(r);
                    }

                    HRESULT Save( const std::string& containerName, const std::string& blobName, size_t dataSize, const uint8_t* blobData )
                    {
                        BlobDataMap map;
                        std::vector<uint8_t> data(dataSize);
                        memcpy_s(&data[0], dataSize, blobData, dataSize);
                        map.emplace(blobName, std::move(data));
                        auto f = m_provider.SaveData(containerName, map);
                        return f.get();
                    }

                    HRESULT Save( const std::string& containerName, const BlobDataMap& map)
                    {
                        auto f = m_provider.SaveData(containerName, map);
                        return f.get();
                    }

                    HRESULT Delete(const std::string& containerName)
                    {
                        auto f = m_provider.DeleteContainer(containerName);
                        return f.get();
                    }
                    
                    HRESULT Delete(const std::string& containerName, const std::string& blobName)
                    {
                        BlobNames toDelete;
                        toDelete.push_back(blobName);
                        return Delete(containerName, toDelete);
                    }
                    
                    HRESULT Delete(const std::string& containerName, const BlobNames& toDelete)
                    {
                        BlobDataMap map;
                        auto f = m_provider.SaveData(containerName, map, toDelete);
                        return f.get();
                    }
                    
                private:
                    Details::XGameSaveProvider m_provider;
                };
                
                class ProviderAsync
                {
                public:
                    std::future<HRESULT> Initialize(XUserHandle userHandle, const std::string& serviceConfigurationId)
                    {
                        return m_provider.Initialize(userHandle, serviceConfigurationId);
                    }
                    
                    std::future<QuotaResult> GetQuota()
                    {
                        return m_provider.GetQuota();
                    }
                    
                    std::future<QueryContainersResult> QueryContainers(const std::string& prefix)
                    {
                        return m_provider.QueryContainers(prefix);
                    }

                    std::future<BlobInfoResult> QueryContainerBlobs(const std::string& containerName)
                    {
                        return m_provider.QueryBlobInfo(containerName);
                    }

                    std::future<BlobResult> Load( const std::string& containerName, const std::string& blobName )
                    {
                        return m_provider.LoadData(containerName, blobName);
                    }

                    std::future<HRESULT> Save( const std::string& containerName, const std::string& blobName, size_t dataSize, const uint8_t* blobData )
                    {
                        BlobDataMap map;
                        std::vector<uint8_t> data(dataSize);
                        memcpy_s(&data[0], dataSize, blobData, dataSize);
                        map.emplace(blobName, std::move(data));
                        return m_provider.SaveData(containerName, map);
                    }

                    std::future<HRESULT> Save( const std::string& containerName, const BlobDataMap& map)
                    {
                        return m_provider.SaveData(containerName, map);
                    }

                    std::future<HRESULT> Delete(const std::string& containerName)
                    {
                        return m_provider.DeleteContainer(containerName);
                    }
                    
                    std::future<HRESULT> Delete(const std::string& containerName, const std::string& blobName)
                    {
                        BlobNames toDelete;
                        toDelete.push_back(blobName);
                        return Delete(containerName, toDelete);
                    }
                    
                    std::future<HRESULT> Delete(const std::string& containerName, const BlobNames& toDelete)
                    {
                        BlobDataMap map;
                        return m_provider.SaveData(containerName, map, toDelete);
                    }

                private:
                    Details::XGameSaveProvider m_provider;
                };
            } //GameSave
            
        }
    }
}
