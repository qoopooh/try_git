== Install ==
* brew install gradle
* sdk install gradle 3.0

== Help ==
* gradle help --task :init

== Init project ==
* gradle init --type java-library
* Remove Library.java & LibraryTest.java
  rm src/main/java/Library.java src/test/java/LibraryTest.java
* add code to build.gradle
```groovy
buildscript {
  ext.kotlin_version = '1.1.3-2'

  repositories {
    maven { url "https://dl.bintray.com/kotlin/kotlin-dev" }
    mavenCentral()
  }

  dependencies {
    classpath "org.jetbrains.kotlin:kotlin-gradle-plugin:$kotlin_version"
  }
}
```

