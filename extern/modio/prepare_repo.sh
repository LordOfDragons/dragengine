#!/bin/bash
find modio-sdk -name ".git*" | xargs -- rm -rf
find modio-sdk -type d -name ".ci" | xargs -- rm -rf
rm -rf modio-sdk/doc
rm -rf modio-sdk/ext/function2/test/googletest
rm -rf modio-sdk/ext/utfcpp/extern/gtest
rm -rf modio-sdk/ext/mbedtls/tests
rm -rf modio-sdk/ext/mbedtls/programs
rm -rf modio-sdk/ext/mbedtls/docs
rm -rf modio-sdk/ext/asio/asio/src/doc
rm -rf modio-sdk/ext/asio/asio/src/tests
rm -rf modio-sdk/ext/asio/asio/src/examples
rm -rf modio-sdk/ext/concurrentqueue/benchmarks
rm -rf modio-sdk/ext/concurrentqueue/tests
rm -rf modio-sdk/ext/json/docs
rm -rf modio-sdk/ext/json/tests
rm -rf modio-sdk/ext/fmt/test
rm -rf modio-sdk/ext/fmt/doc
rm -rf modio-sdk/ext/filesystem/test
rm -rf modio-sdk/ext/filesystem/examples
