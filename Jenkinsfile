pipeline {
    agent any

    environment {
        ARDUINO_BOARD = 'esp32:esp32:XIAO_ESP32C3'
        SKETCH = 'src/src.ino'
        CREDENTIALS_PATH = '/var/lib/jenkins/credentials/credentials.h'
        ARDUINO_CLI_PATH = '/var/lib/jenkins/workspace/Arduino ESP32-C3 CI-CD/bin'
        LIBRARIES_PATH = '/var/lib/jenkins/Arduino/libraries'
        UPDATES_SERVER_DIR = '/var/www/updates'
        COMPILED_BINARY_PATH = '/var/lib/jenkins/.cache/arduino/sketches/F7866680E5E7F68088A4CBE4A3C7E26F/src.ino.bin'
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
        }        ngrok http 8080        sudo rm /usr/local/bin/ngrok        sudo mv ngrok /usr/local/bin/
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
        stage('Compile Sketch') {
            steps {
                sh "arduino-cli compile --fqbn ${ARDUINO_BOARD} --libraries ${LIBRARIES_PATH} --build-property build.partitions=min_spiffs --build-property upload.maximum_size=1966080 --verbose ${SKETCH}"
            }
        }
        stage('Upload to Updates Server') {
            steps {
                sh "cp ${COMPILED_BINARY_PATH} ${UPDATES_SERVER_DIR}/"
            }
        }
    }
}