plugins {
    id("com.android.application")
    id("kotlin-android")
    // Il plugin Flutter deve essere applicato dopo i plugin Android e Kotlin.
    id("dev.flutter.flutter-gradle-plugin")
}

android {
    namespace = "com.example.apolline"
    compileSdk = flutter.compileSdkVersion
    //ndkVersion = flutter.ndkVersion
    compileSdkVersion (34) // Add this line

    // Imposta esplicitamente la versione NDK richiesta
    ndkVersion = "27.0.12077973"

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
        // Abilita il core library desugaring
        isCoreLibraryDesugaringEnabled = true
    }

    kotlinOptions {
        jvmTarget = JavaVersion.VERSION_11.toString()
    }

    defaultConfig {
        applicationId = "com.example.apolline"
        minSdk = flutter.minSdkVersion
        targetSdk = flutter.targetSdkVersion
        versionCode = flutter.versionCode
        versionName = flutter.versionName
    }

    buildTypes {
        release {
            // Usa la signing config di debug per release (sostituisci con la tua firma reale se necessario)
            signingConfig = signingConfigs.getByName("debug")
        }
    }
}

dependencies {
    // Aggiungi la dipendenza per il desugaring
    coreLibraryDesugaring("com.android.tools:desugar_jdk_libs:1.2.2")
}

flutter {
    source = "../.."
}
