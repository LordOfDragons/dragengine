import com.android.build.gradle.internal.tasks.factory.dependsOn

plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
}

android {
    namespace = "ch.dragondreams.delauncher"
    compileSdk = 34

    defaultConfig {
        applicationId = "ch.dragondreams.delauncher"
        minSdk = 26
        targetSdk = 34

        versionCode = (findProperty("build.version.code") as String? ?: "99999").toInt()
        versionName = findProperty("build.version.name") as String? ?: "99999"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags += ""
                arguments += "-DANDROID_STL=c++_shared"
            }
        }
        ndk {
            abiFilters.clear()
        }
    }

    buildTypes {
        debug {
            isDebuggable = true
            isJniDebuggable = true
        }
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            //signingConfig = signingConfigs.getByName("debug")
        }
    }

    flavorDimensions += "architecture"

    productFlavors {
        create("armv8") {
            dimension = "architecture"
            versionNameSuffix = "-armv8"
            ndk {
                abiFilters += "arm64-v8a"
            }
            externalNativeBuild {
                cmake {
                    arguments += "-DANDROID_FLAVOR=armv8"
                }
            }
        }
        create("armv7") {
            dimension = "architecture"
            versionNameSuffix = "-armv7"
            ndk {
                abiFilters += "armeabi-v7a"
            }
            externalNativeBuild {
                cmake {
                    arguments += "-DANDROID_FLAVOR=armv7"
                }
            }
        }
        create("quest") {
            dimension = "architecture"
            versionNameSuffix = "-quest"
            ndk {
                abiFilters += "arm64-v8a"
            }
            externalNativeBuild {
                cmake {
                    arguments += "-DANDROID_FLAVOR=armv8"
                }
            }
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }

    androidResources {
        noCompress += ".delga"
        noCompress += ".zip"
    }

    buildFeatures {
        viewBinding = true

        // To use the Android Frame Pacing or Android Performance Tuner libraries, enable
        // native dependencies to be imported. Libraries will be made available to your CMake build
        // as packages named "games-frame-pacing" and "games-performance-tuner".
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

task(name = "buildEngineAsset", type = Exec::class) {
    doFirst {
        println("Build Engine Asset ...")
    }
    workingDir("..")
    executable("./copy_resources.sh")
}

gradle.projectsEvaluated {
    tasks.named("generateArmv7DebugAssets").dependsOn("buildEngineAsset")
    tasks.named("generateArmv8DebugAssets").dependsOn("buildEngineAsset")
    tasks.named("generateQuestDebugAssets").dependsOn("buildEngineAsset")
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.constraintlayout)
    implementation(libs.androidx.lifecycle.livedata.ktx)
    implementation(libs.androidx.lifecycle.viewmodel.ktx)
    implementation(libs.androidx.legacy.support.v4)
    implementation(libs.androidx.recyclerview)
    implementation(libs.androidx.activity)
    implementation(libs.androidx.games.activity)
    implementation(libs.androidx.games.controller)
    implementation(libs.androidx.games.frame.pacing)
    implementation(libs.androidx.games.memory.advice)
    implementation(libs.androidx.games.performance.tuner)
    implementation(libs.androidx.preference)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}
