import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts

Popup {
    id: root

    //property string bookId
    //property string title
    // property string authors
    // property string languages
    // property var cover
    // property int downloadCount
    // property string downloadLink
    //property string description
    property string popupTitle: qsTr( "Add OPDS catalog")
    property color titleColor: Style.colorTitle
    property string titleFieldText: qsTr("Library title")
    property string urlFieldText: "https://"
    implicitWidth: 751
    implicitHeight: layout.height

    padding: 0
    background: Rectangle {
        radius: 6
        color: Style.colorPopupBackground
    }

    modal: true
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onOpened: {
        pageTitle.text = root.popupTitle
        pageTitle.color = root.titleColor
        titleField.text = root.titleFieldText
        urlField.text = root.urlFieldText
        saveButton.forceActiveFocus()
    }

    Connections {
        target: OpdsController
        function onBadOpdsLibData(strErr) {
            internal.errorSaving(strErr)
        }

        function onSavingLibFinished() {
            addOpdsLibPopup.close()
        }
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            id: layout
            width: parent.width

            spacing: 0

            MButton {
                id: closePopupButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: 14
                Layout.alignment: Qt.AlignRight
                backgroundColor: "transparent"
                borderColor: "transparent"
                opacityOnPressed: 0.7
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14

                onClicked: root.close()
            }

            Pane {
                id: content
                Layout.fillWidth: true
                horizontalPadding: 52
                bottomPadding: 42
                background: Rectangle {
                    color: "transparent"
                    radius: 6
                }

                ColumnLayout {
                    id: contentLayout
                    width: parent.width
                    spacing: 0

                    Label {
                        id: pageTitle
                        Layout.topMargin: 6
                        text: root.popupTitle
                        font.weight: Font.Bold
                        font.pointSize: Fonts.size19
                        color: root.titleColor
                    }

                    ColumnLayout {
                        id: bookDetails
                        Layout.fillWidth: true
                        Layout.topMargin: 30
                        spacing: 16

                        MLabeledInputBox {
                            id: titleField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Title"
                            headerFontWeight: Font.Bold
                            headerFontSize: Fonts.size11dot5
                            text: root.titleFieldText
                            headerToBoxSpacing: 3
                            inputFontSize: Fonts.size12
                            inputFontColor: Style.colorReadOnlyInputText
                            textPadding: 12
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: false
                        }

                        MLabeledInputBox {
                            id: urlField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Url"
                            headerFontWeight: Font.Bold
                            headerFontSize: Fonts.size11dot5
                            text: root.urlFieldText
                            headerToBoxSpacing: 3
                            inputFontSize: Fonts.size12
                            inputFontColor: Style.colorReadOnlyInputText
                            textPadding: 12
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: false
                        }

                        MLabeledInputBox {
                            id: descrField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Description"
                            headerFontWeight: Font.Bold
                            headerFontSize: Fonts.size11dot5
                            text: "Library descripction"
                            headerToBoxSpacing: 3
                            inputFontSize: Fonts.size12
                            inputFontColor: Style.colorReadOnlyInputText
                            textPadding: 12
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: false
                        }

                        RowLayout {
                            id: buttonsLayout
                            Layout.topMargin: 42
                            spacing: 16

                            MButton {
                                id: saveButton
                                Layout.preferredWidth: 140
                                Layout.preferredHeight: 38
                                active: true
                                text: "Save"
                                textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                                fontWeight: Font.Bold
                                fontSize: Fonts.size12
                                borderWidth: active ? 0 : 1
                                backgroundColor: active ? Style.colorBasePurple : "transparent"
                                imagePath: active ? Icons.downloadSelected : Icons.download
                                imageSize: 18

                                onClicked: {
                                    internal.saveLib()
                                    //root.close()
                                }
                                Keys.onReturnPressed: internal.saveLib()
                                Keys.onRightPressed: internal.giveFocusToCancelButton()
                                Keys.onTabPressed: internal.giveFocusToCancelButton()
                            }

                            MButton {
                                id: cancelButton
                                Layout.preferredWidth: 140
                                Layout.preferredHeight: 38
                                borderWidth: active ? 0 : 1
                                backgroundColor: active ? Style.colorBasePurple : "transparent"
                                opacityOnPressed: 0.7
                                text: "Cancel"
                                textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                                fontWeight: Font.Bold
                                fontSize: Fonts.size12

                                onClicked: root.close()
                                Keys.onReturnPressed: root.close()
                                Keys.onLeftPressed: internal.giveFocusToSaveButton()
                                Keys.onTabPressed: internal.giveFocusToSaveButton()
                            }
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal

        function saveLib() {
            // TODO Save lib to json file
            OpdsController.saveOpdsLib(titleField.text, urlField.text,
                                       descrField.text)
        }

        function errorSaving(str) {
            pageTitle.text = "Error"
            pageTitle.color = "#FF0000"
            urlField.text = str
            titleField.text = str
        }

        function giveFocusToCancelButton() {
            saveButton.active = false
            cancelButton.active = true
            cancelButton.giveFocus()
        }

        function giveFocusToSaveButton() {
            cancelButton.active = false
            saveButton.active = true
            saveButton.giveFocus()
        }
    }
}
