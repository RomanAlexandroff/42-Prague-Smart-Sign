pipeline {
    agent any

    environment {
        ARDUINO_BOARD = 'esp32:esp32:esp32c3'
        SKETCH = 'src/src.ino'
        SERIAL_PORT = '/dev/ttyUSB0'
        CREDENTIALS_PATH = '/home/roman/42-Prague-Smart-Sign/src/credentials.h'
    }

    stages {
        stage('Checkout') {
            steps {
                git branch: 'main', url: 'https://github.com/RomanAlexandroff/42-Prague-Smart-Sign.git'
            }
        }
        stage('Install Arduino CLI') {
            steps {
                sh 'curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh'
                sh 'sudo mv bin/arduino-cli /usr/local/bin/'
            }
        }
        stage('Configure Arduino CLI') {
            steps {
                sh 'arduino-cli config init'
                sh 'arduino-cli core update-index'
                sh 'arduino-cli core install esp32:esp32'
            }
        }
        stage('Prepare Project') {
            steps {
                sh "cp ${CREDENTIALS_PATH} src/"
            }
        }
        stage('Compile Sketch') {
            steps {
                sh "arduino-cli compile --fqbn ${ARDUINO_BOARD} ${SKETCH}"
            }
        }
        stage('Upload to Updates Server') {
            steps {
                sh "curl -u username:password -T build/${SKETCH}.bin http://10.0.1.31/updates/"
            }
        }
    }
}