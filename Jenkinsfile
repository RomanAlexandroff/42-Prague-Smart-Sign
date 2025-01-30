pipeline {
    agent any

    environment {
        ARDUINO_BOARD = 'esp32:esp32:XIAO_ESP32C3'
        SKETCH = 'src/src.ino'
        SERIAL_PORT = '/dev/ttyUSB0'
        CREDENTIALS_PATH = '/var/lib/jenkins/credentials/credentials.h'
        ARDUINO_CLI_PATH = '/var/lib/jenkins/workspace/Arduino ESP32-C3 CI-CD/bin'
        LIBRARIES_PATH = '/home/roman/Arduino/libraries'
    }

    stages {
        stage('Checkout') {
            steps {
                git branch: 'main', url: 'https://github.com/RomanAlexandroff/42-Prague-Smart-Sign.git'
            }
        }
        stage('Install Arduino CLI') {
            steps {
                script {
                    if (!fileExists("${ARDUINO_CLI_PATH}/arduino-cli")) {
                        sh 'curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh'
                        sh 'export PATH=$PATH:${ARDUINO_CLI_PATH}'
                    } else {
                        echo 'Arduino CLI already installed'
                    }
                }
            }
        }
        stage('Configure Arduino CLI') {
            steps {
                script {
                    def coreInstalled = sh(script: 'arduino-cli core list | grep esp32', returnStatus: true) == 0
                    if (!coreInstalled) {
                        sh 'arduino-cli core update-index'
                        sh 'arduino-cli core install esp32:esp32'
                    } else {
                        echo 'ESP32 core already installed'
                    }
                }
            }
        }
        stage('Prepare Project') {
            steps {
                sh "cp ${CREDENTIALS_PATH} src/"
            }
        }
        stage('Print Environment Variables') {
            steps {
                sh 'printenv'
            }
        }
        stage('Compile Sketch') {
            steps {
                sh "arduino-cli compile --fqbn ${ARDUINO_BOARD} --libraries ${LIBRARIES_PATH} --build-properties build.partitions=min_spiffs,upload.maximum_size=1966080 --verbose ${SKETCH}"
            }
        }
        stage('Upload to Updates Server') {
            steps {
                sh "curl -u username:password -T build/${SKETCH}.bin http://10.0.1.31/updates/"
            }
        }
    }
}