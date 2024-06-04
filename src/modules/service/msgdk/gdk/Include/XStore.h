// Copyright (c) Microsoft Corporation.  All rights reserved

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <stdint.h>
#include <XAsync.h>
#include <XPackage.h>
#include <XTaskQueue.h>
#include <XUser.h>

extern "C"
{

#define IN_APP_OFFER_TOKEN_MAX_SIZE (64)
#define PRICE_MAX_SIZE (16)
#define TRIAL_UNIQUE_ID_MAX_SIZE (64)
#define SKU_ID_SIZE (5)
#define STORE_SKU_ID_SIZE (18)

enum class XStoreCanLicenseStatus : uint32_t
{
    NotLicensableToUser                 = 0,
    Licensable                          = 1,
    LicenseActionNotApplicableToProduct = 2,
};

enum class XStoreProductKind : uint32_t
{
    None                = 0x00,
    Consumable          = 0x01,
    Durable             = 0x02,
    Game                = 0x04,
    Pass                = 0x08,
    UnmanagedConsumable = 0x10,
};

DEFINE_ENUM_FLAG_OPERATORS(XStoreProductKind);

enum class XStoreDurationUnit : uint32_t
{
    Minute  = 0,
    Hour    = 1,
    Day     = 2,
    Week    = 3,
    Month   = 4,
    Year    = 5,
};

struct XStoreImage
{
    _Field_z_ const char* uri;
    uint32_t height;
    uint32_t width;
    _Field_z_ const char* caption;
    _Field_z_ const char* imagePurposeTag;
};

struct XStorePrice
{
    float basePrice;
    float price;
    float recurrencePrice;
    _Field_z_ const char* currencyCode;
    _Field_z_ char formattedBasePrice[PRICE_MAX_SIZE];
    _Field_z_ char formattedPrice[PRICE_MAX_SIZE];
    _Field_z_ char formattedRecurrencePrice[PRICE_MAX_SIZE];
    bool isOnSale;
    time_t saleEndDate;
};

struct XStoreAvailability
{
    _Field_z_ const char* availabilityId;
    XStorePrice price;
    time_t endDate;
};

struct XStoreCollectionData
{
    time_t acquiredDate;
    time_t startDate;
    time_t endDate;
    bool isTrial;
    uint32_t trialTimeRemainingInSeconds;
    uint32_t quantity;
    _Field_z_ const char* campaignId;
    _Field_z_ const char* developerOfferId;
};

struct XStoreSubscriptionInfo
{
    bool hasTrialPeriod;
    XStoreDurationUnit trialPeriodUnit;
    uint32_t trialPeriod;
    XStoreDurationUnit billingPeriodUnit;
    uint32_t billingPeriod;
};

struct XStoreVideo
{
    _Field_z_ const char* uri;
    uint32_t height;
    uint32_t width;
    _Field_z_ const char* caption;
    _Field_z_ const char* videoPurposeTag;
    XStoreImage previewImage;
};

struct XStoreSku
{
    _Field_z_ const char* skuId;
    _Field_z_ const char* title;
    _Field_z_ const char* description;
    _Field_z_ const char* language;
    XStorePrice price;
    bool isTrial;
    bool isInUserCollection;
    XStoreCollectionData collectionData;
    bool isSubscription;
    XStoreSubscriptionInfo subscriptionInfo;
    uint32_t bundledSkusCount;
    _Field_z_ const char** bundledSkus;
    uint32_t imagesCount;
    XStoreImage* images;
    uint32_t videosCount;
    XStoreVideo* videos;
    uint32_t availabilitiesCount;
    XStoreAvailability* availabilities;
};

struct XStoreProduct
{
    _Field_z_ const char* storeId;
    _Field_z_ const char* title;
    _Field_z_ const char* description;
    _Field_z_ const char* language;
    _Field_z_ const char* inAppOfferToken;
    _Field_z_ char* linkUri;
    XStoreProductKind productKind;
    XStorePrice price;
    bool hasDigitalDownload;
    bool isInUserCollection;
    uint32_t keywordsCount;
    _Field_z_ const char** keywords;
    uint32_t skusCount;
    XStoreSku* skus;
    uint32_t imagesCount;
    XStoreImage* images;
    uint32_t videosCount;
    XStoreVideo* videos;
};

struct XStoreGameLicense
{
    _Field_z_ char skuStoreId[STORE_SKU_ID_SIZE];
    bool isActive;
    bool isTrialOwnedByThisUser;
    bool isDiscLicense;
    bool isTrial;
    uint32_t trialTimeRemainingInSeconds;
    _Field_z_ char trialUniqueId[TRIAL_UNIQUE_ID_MAX_SIZE];
    time_t expirationDate;
};

struct XStoreAddonLicense
{
    _Field_z_ char skuStoreId[STORE_SKU_ID_SIZE];
    _Field_z_ char inAppOfferToken[IN_APP_OFFER_TOKEN_MAX_SIZE];
    bool isActive;
    time_t expirationDate;
};

struct XStorePackageUpdate
{
    _Field_z_ char packageIdentifier[XPACKAGE_IDENTIFIER_MAX_LENGTH];
    bool isMandatory;
};

struct XStoreCanAcquireLicenseResult
{
    _Field_z_ char licensableSku[SKU_ID_SIZE];
    XStoreCanLicenseStatus status;
};

struct XStoreConsumableResult
{
    uint32_t quantity;
};

struct XStoreRateAndReviewResult
{
    bool wasUpdated;
};

typedef struct XStoreContext* XStoreContextHandle;
typedef struct XStoreProductQuery* XStoreProductQueryHandle;
typedef struct XStoreLicense* XStoreLicenseHandle;

typedef bool CALLBACK XStoreProductQueryCallback(_In_ const XStoreProduct* product, _In_opt_ void* context);
typedef void CALLBACK XStorePackageLicenseLostCallback(_In_ void* context);
typedef void CALLBACK XStoreGameLicenseChangedCallback(_In_ void* context);

STDAPI XStoreCreateContext(
    _In_opt_ const XUserHandle user,
    _Out_ XStoreContextHandle* storeContextHandle
    ) noexcept;

STDAPI_(void) XStoreCloseContextHandle(
    _In_ XStoreContextHandle storeContextHandle
    ) noexcept;

STDAPI XStoreQueryAssociatedProductsAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_ XStoreProductKind productKinds,
    _In_ uint32_t maxItemsToRetrievePerPage,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryAssociatedProductsResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI XStoreQueryProductsAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_ XStoreProductKind productKinds,
    _In_z_count_(storeIdsCount) const char** storeIds,
    _In_ size_t storeIdsCount,
    _In_opt_z_count_(actionFiltersCount) const char** actionFilters,
    _In_ size_t actionFiltersCount,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryProductsResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI XStoreQueryEntitledProductsAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_ XStoreProductKind productKinds,
    _In_ uint32_t maxItemsToRetrievePerPage,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryEntitledProductsResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI XStoreQueryProductForCurrentGameAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryProductForCurrentGameResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI XStoreQueryProductForPackageAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_ XStoreProductKind productKinds,
    _In_z_ const char* packageIdentifier,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryProductForPackageResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI XStoreEnumerateProductsQuery(
    _In_ const XStoreProductQueryHandle productQueryHandle,
    _In_opt_ void* context,
    _In_ XStoreProductQueryCallback* callback
    ) noexcept;

STDAPI_(bool) XStoreProductsQueryHasMorePages(
    _In_ const XStoreProductQueryHandle productQueryHandle
    ) noexcept;

STDAPI XStoreProductsQueryNextPageAsync(
    _In_ const XStoreProductQueryHandle productQueryHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreProductsQueryNextPageResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;

STDAPI_(void) XStoreCloseProductsQueryHandle(
    _In_ XStoreProductQueryHandle productQueryHandle
    ) noexcept;

STDAPI XStoreAcquireLicenseForPackageAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* packageIdentifier,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreAcquireLicenseForPackageResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreLicenseHandle* storeLicenseHandle
    ) noexcept;

STDAPI_(bool) XStoreIsLicenseValid(
    _In_ const XStoreLicenseHandle storeLicenseHandle
    ) noexcept;

STDAPI_(void) XStoreCloseLicenseHandle(
    _In_ XStoreLicenseHandle storeLicenseHandle
    ) noexcept;

STDAPI XStoreCanAcquireLicenseForStoreIdAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeProductId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreCanAcquireLicenseForStoreIdResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreCanAcquireLicenseResult* storeCanAcquireLicense
    ) noexcept;

STDAPI XStoreCanAcquireLicenseForPackageAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* packageIdentifier,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreCanAcquireLicenseForPackageResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreCanAcquireLicenseResult* storeCanAcquireLicense
    ) noexcept;

STDAPI XStoreQueryGameLicenseAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryGameLicenseResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreGameLicense* license
    ) noexcept;

STDAPI XStoreQueryAddOnLicensesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryAddOnLicensesResultCount(
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* count
    ) noexcept;

STDAPI XStoreQueryAddOnLicensesResult(
    _Inout_ XAsyncBlock* async,
    _In_ uint32_t count,
    _Out_writes_(count) XStoreAddonLicense* addOnLicenses
    ) noexcept;

STDAPI XStoreQueryConsumableBalanceRemainingAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeProductId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryConsumableBalanceRemainingResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreConsumableResult* consumableResult
    ) noexcept;

STDAPI XStoreReportConsumableFulfillmentAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeProductId,
    _In_ uint32_t quantity,
    _In_ GUID trackingId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreReportConsumableFulfillmentResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreConsumableResult* consumableResult
    ) noexcept;

STDAPI XStoreGetUserCollectionsIdAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* serviceTicket,
    _In_z_ const char* publisherUserId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreGetUserCollectionsIdResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* size
    ) noexcept;

STDAPI XStoreGetUserCollectionsIdResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t size,
    _Out_writes_z_(size) char* result
    ) noexcept;

STDAPI XStoreGetUserPurchaseIdAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* serviceTicket,
    _In_z_ const char* publisherUserId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreGetUserPurchaseIdResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* size
    ) noexcept;

STDAPI XStoreGetUserPurchaseIdResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t size,
    _Out_writes_z_(size) char* result
    ) noexcept;

STDAPI XStoreQueryLicenseTokenAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_count_(productIdsCount) const char** productIds,
    _In_ size_t productIdsCount,
    _In_z_ const char* customDeveloperString,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryLicenseTokenResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* size
    ) noexcept;

STDAPI XStoreQueryLicenseTokenResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t size,
    _Out_writes_z_(size) char* result
    ) noexcept;

STDAPI XStoreShowPurchaseUIAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeId,
    _In_opt_z_ const char* name,
    _In_opt_z_ const char* extendedJsonData,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowPurchaseUIResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowProductPageUIAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowProductPageUIResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowAssociatedProductsUIAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeId,
    _In_ XStoreProductKind productKinds,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowAssociatedProductsUIResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowRateAndReviewUIAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowRateAndReviewUIResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreRateAndReviewResult* result
    ) noexcept;

STDAPI XStoreShowRedeemTokenUIAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* token,
    _In_z_count_(allowedStoreIdsCount) const char** allowedStoreIds,
    _In_ size_t allowedStoreIdsCount,
    _In_ bool disallowCsvRedemption,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreShowRedeemTokenUIResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryGameAndDlcPackageUpdatesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryGameAndDlcPackageUpdatesResultCount(
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* count
    ) noexcept;

STDAPI XStoreQueryGameAndDlcPackageUpdatesResult(
    _Inout_ XAsyncBlock* async,
    _In_ uint32_t count,
    _Out_writes_(count) XStorePackageUpdate* packageUpdates
    ) noexcept;

STDAPI XStoreQueryPackageUpdatesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_ const char** packageIdentifiers,
    _In_ size_t packageIdentifiersCount,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryPackageUpdatesResultCount(   
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* count
    ) noexcept;

STDAPI XStoreQueryPackageUpdatesResult(   
    _Inout_ XAsyncBlock* async,
    _In_ uint32_t count,
    _Out_writes_(count) XStorePackageUpdate* packageUpdates 
    ) noexcept;

STDAPI XStoreDownloadPackageUpdatesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_count_(packageIdentifiersCount) const char** packageIdentifiers,
    _In_ size_t packageIdentifiersCount,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreDownloadPackageUpdatesResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreDownloadAndInstallPackageUpdatesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_count_(packageIdentifiersCount) const char** packageIdentifiers,
    _In_ size_t packageIdentifiersCount,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreDownloadAndInstallPackageUpdatesResult(
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreDownloadAndInstallPackagesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_count_(storeIdsCount) const char** storeIds,
    _In_ size_t storeIdsCount,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreDownloadAndInstallPackagesResultCount(
    _In_ XAsyncBlock* async,
    _Out_ uint32_t* count
    ) noexcept;

STDAPI XStoreDownloadAndInstallPackagesResult(
    _Inout_ XAsyncBlock* async,
    _In_ uint32_t count,
    _Out_writes_z_(count) char packageIdentifiers[][XPACKAGE_IDENTIFIER_MAX_LENGTH]
    ) noexcept;

STDAPI XStoreQueryPackageIdentifier(
    _In_z_ const char* storeId,
    _In_ size_t size,
    _Out_writes_z_(size) char* packageIdentifier
    ) noexcept;

STDAPI XStoreRegisterGameLicenseChanged(
    _In_ XStoreContextHandle storeContextHandle,
    _In_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XStoreGameLicenseChangedCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XStoreUnregisterGameLicenseChanged(
    _In_ XStoreContextHandle storeContextHandle,
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

STDAPI XStoreRegisterPackageLicenseLost(
    _In_ XStoreLicenseHandle licenseHandle,
    _In_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ XStorePackageLicenseLostCallback* callback,
    _Out_ XTaskQueueRegistrationToken* token
    ) noexcept;

STDAPI_(bool) XStoreUnregisterPackageLicenseLost(
    _In_ XStoreLicenseHandle licenseHandle,
    _In_ XTaskQueueRegistrationToken token,
    _In_ bool wait
    ) noexcept;

STDAPI_(bool) XStoreIsAvailabilityPurchasable(
    _In_ const XStoreAvailability availability
    ) noexcept;

STDAPI XStoreAcquireLicenseForDurablesAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeId,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreAcquireLicenseForDurablesResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreLicenseHandle* storeLicenseHandle
    ) noexcept;

STDAPI XStoreQueryAssociatedProductsForStoreIdAsync(
    _In_ const XStoreContextHandle storeContextHandle,
    _In_z_ const char* storeProductId,
    _In_ XStoreProductKind productKinds,
    _In_ uint32_t maxItemsToRetrievePerPage,
    _Inout_ XAsyncBlock* async
    ) noexcept;

STDAPI XStoreQueryAssociatedProductsForStoreIdResult(
    _Inout_ XAsyncBlock* async,
    _Out_ XStoreProductQueryHandle* productQueryHandle
    ) noexcept;
}
