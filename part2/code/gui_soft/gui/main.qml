import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Window {
    id: mainWindow
    objectName: "mainWindow"

    width: 640
    height: 480
    visible: true
    title: qsTr("Math Computer GUI")

    signal setStatus(string statusText)

    onSetStatus: {
        status.text = statusText;
    }

    signal setResult(string resultText)

    onSetResult: {
        textResult.text = resultText
    }

    signal setNbCompute(string resultText)

    onSetNbCompute: {
        textNbCompute.text = resultText
    }

    ColumnLayout {

        RowLayout {

            GridLayout {
                columns: 2

                Label {
                    text : "A: "
                }

                TextField {
                    id: textA
                    objectName: "textA"
                    placeholderText: "Number to be used for computation"
                    onTextChanged: controller.setA(text)
                }

                Label {
                    text : "B: "
                }

                TextField {
                    objectName: "textB"
                    placeholderText: "Number to be used for computation"
                    onTextChanged: controller.setB(text)
                }

                Label {
                    text : "C: "
                }

                TextField {
                    objectName: "textC"
                    placeholderText: "Number to be used for computation"
                    onTextChanged: controller.setC(text)
                }


                Label {
                    text : "Result: "
                }

                TextField {
                    id: textResult
                    objectName: "textResult"
                    readOnly: true
                }


                Label {
                    text : "Nb computations: "
                }

                TextField {
                    id: textNbCompute
                    objectName: "textNbCompute"
                    readOnly: true
                }
            }

            ColumnLayout {

                Button {
                    id: computeButton
                    text: "Compute"
                    objectName: "computeButton"
                    onClicked: controller.compute()
                }


                Button {
                    id: getNbComputeButton
                    text: "Get Nb computations"
                    objectName: "getNbComputeButton"
                    onClicked: controller.getNbCompute()
                }


                Button {
                    id: resetNbComputeButton
                    text: "Reset Nb computations"
                    objectName: "resetNbComputeButton"
                    onClicked: controller.resetNbCompute()
                }

            }
        }

        RowLayout {
            Label {
                text: "Status: "
            }

            Label {
                id: status
                objectName: "status"
                text: ""
            }
        }

    }
}
