import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.controllers
import Librum.style
import Librum.fonts
import Librum.icons
import CustomComponents

Page {
    id: root
    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }

    Connections {
        target: OpdsController

        function onBadNetworkResponse(code) {
            if (code===-100){
                  errorMessageLabel.text = qsTr("Sorry.Unsupported format.");
            }
            else{
                errorMessageLabel.text = qsTr("Bad network response: ") + code

            }
            errorMessageLabel.visible = true
            timer.start()

        }
    }
    Connections {
        target: LibraryController

        function onAddingOpdsBookFinished(id, result) {
            if (result) {
                OpdsController.markBookAsDownloaded(id)
            }
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.rightMargin: internal.windowRightMargin
        anchors.leftMargin: internal.windowLeftMargin
        spacing: 0

        RowLayout {
            // toolbar
            id: headerRow
            Layout.fillWidth: true
            spacing: 0

            MTitle {
                id: pageTitle
                Layout.topMargin: 44
                titleText: qsTr("OPDS  catalogs")
                descriptionText: qsTr("Find books in public OPDS libraries")
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                id: addOpdsButton
                Layout.preferredWidth: 220
                Layout.preferredHeight: 40
                Layout.topMargin: 22
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: qsTr("Add OPDS")
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Bold
                fontSize: Fonts.size13
                imagePath: Icons.addWhite

                onClicked: addOpdsLibPopup.open()
            }
        } // toolbar

        Label {
            id: errorMessageLabel
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: -sidebar.width
            Layout.topMargin: Math.round(root.height / 5)
            color: Style.colorTitle
            font.pointSize: Fonts.size22
            font.weight: Font.Medium
            visible: false
        }
        Timer {
            id: timer
            interval: 1000
            repeat: false
            running: false
            onTriggered: errorMessageLabel.visible = false
        }

        Pane {
            id: opdsGridContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 30
            visible: !root.empty
            padding: 0
            background: Rectangle {
                color: "transparent"
            }

            GridView {
                id: opdsGrid
                anchors.fill: parent
                cellWidth: internal.folderWidth + internal.horizontalFolderSpacing
                cellHeight: internal.folderHeight + internal.verticalFolderSpacing
                interactive: true
                clip: true
                cacheBuffer: 1000
                model: OpdsController.opdsModel
                delegate: MOpdsNode {
                    MouseArea {
                        id: clickArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            parent.scale = 1.1
                        }
                        onExited: {
                            parent.scale = 1.0
                        }

                        onClicked: {
                            if (model.downloadUrl !== "") {
                                downloadOpdsBookPopup.bookId = model.id
                                downloadOpdsBookPopup.title = model.title
                                downloadOpdsBookPopup.authors = model.author
                                downloadOpdsBookPopup.description = model.descr
                                // downloadOpdsBookPopup.languages = model.languages
                                downloadOpdsBookPopup.cover = model.imgDataReady
                                        === false ? "" : "image://opds_image_provider/"
                                                    + model.imageUrl
                                // downloadOpdsBookPopup.downloadCount = model.downloadCount
                                downloadOpdsBookPopup.downloadLink = model.downloadUrl
                                downloadOpdsBookPopup.open()
                            } else if (model.url !== "") {
                                folderImage.visible = false
                                loadingAnimation.playing = true
                                loadingAnimation.visible = true
                                OpdsController.loadRootLib(model.url)
                            }
                        }
                    } // MouseArea

                    // load images
                    Component.onCompleted: {
                        OpdsController.getNodeImage(model.id)
                    }
                    // Component.onDestruction:  no need to explicitly delete
                } // MOpdsNode
            } // GridView

            ScrollBar {
                id: verticalScrollbar
                width: pressed ? 14 : 12
                hoverEnabled: true
                active: true
                policy: ScrollBar.AlwaysOff
                visible: opdsGrid.contentHeight > opdsGrid.height
                orientation: Qt.Vertical
                size: opdsGrid.height / opdsGrid.contentHeight
                minimumSize: 0.04
                position: (opdsGrid.contentY - opdsGrid.originY) / opdsGrid.contentHeight
                onPositionChanged: if (pressed)
                                       opdsGrid.contentY = position
                                               * opdsGrid.contentHeight + opdsGrid.originY
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: -20
                anchors.bottomMargin: 16
                anchors.bottom: parent.bottom
                horizontalPadding: 4

                contentItem: Rectangle {
                    color: Style.colorScrollBarHandle
                    opacity: verticalScrollbar.pressed ? 0.8 : 1
                    radius: 4
                }

                background: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 200
                    color: "transparent"
                }
            }
        } // Pane
    } // ColumnLayout

    MDownloadOpdsBookPopup {
        id: downloadOpdsBookPopup

        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }

    MOpdsLibAddPopup {
        id: addOpdsLibPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }

    QtObject {
        id: internal
        property int windowLeftMargin: 64
        property int windowRightMargin: 70

        property int folderWidth: 190
        property int folderHeight: 300
        property int horizontalFolderSpacing: 64
        property int verticalFolderSpacing: 48
    }
}
