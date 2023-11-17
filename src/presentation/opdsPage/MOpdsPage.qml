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

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.rightMargin: internal.windowRightMargin
        anchors.leftMargin: internal.windowLeftMargin
        spacing: 0

        RowLayout {
            id: headerRow
            Layout.fillWidth: true
            spacing: 0

            MTitle {
                id: pageTitle
                Layout.topMargin: 44
                titleText: "OPDS  catalogs"
                descriptionText: "Find books in public OPDS libraries"
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                id: addOpdsButton
                Layout.preferredWidth: 140
                Layout.preferredHeight: 40
                Layout.topMargin: 22
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Add OPDS"
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Bold
                fontSize: Fonts.size13
                imagePath: Icons.addWhite

                onClicked: ; // FIXME importFilesDialog.open()
            }
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
                delegate: contactDelegate
                clip: true
                cacheBuffer: 1000
                model: OpdsController.opdsModel
            }
        }

       Component {
            id: contactDelegate
            Item {
                id: delegateItem
                width: internal.folderWidth
                height: internal.folderHeight
                z:0
                Item {
                    anchors.fill: parent
                        Rectangle{
                            id:coverRectangle
                            width:parent.width
                            height: parent.height*0.8;
                            color: Style.colorPageBackground
                            border.color: "grey"

//                        Image {
//                                id: folderIcon
//                                antialiasing: true
//                                asynchronous: true
//                                width: parent.width
//                                fillMode: Image.PreserveAspectFit
//                                source: Icons.opdsFolder
//                                anchors.horizontalCenter: parent.horizontalCenter
//                                anchors.verticalCenter: parent.verticalCenter
//                                visible: !model.imgDataReady
//   } //image
                                AnimatedImage {
                                        id: loadingAnimation
                                        anchors.centerIn: parent
                                        width: parent.width / 2
                                        playing: false
                                        visible: false
                                        fillMode: Image.PreserveAspectFit
                                        source: Icons.loadingAnimation
                                }

                        Image {
                                id: nodeThumbnail
                                asynchronous: true
                                cache: true
                                anchors.top: parent.top
                                width: parent.width
                                height: parent.height
                                fillMode: Image.PreserveAspectFit
                                source: model.imgDataReady
                                        === false ? "" : "image://opds_image_provider/" + model.imageUrl
                                visible: model.imgDataReady
                                sourceSize.height: 240
                                sourceSize.width: 160
                        } //image

                        Text {
                                id: delegateItemText
                                text: model.title
                                maximumLineCount: 3
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.Wrap
                                width: parent.width
                                height: nodeThumbnail.visible ? parent.heigh*0.3 :  parent.height;
                               // height: parent * 0.3
                                padding: 5
                                //anchors.horizontalCenter: coverRectangle.horizontalCenter
                                anchors.top: nodeThumbnail.visible ? nodeThumbnail.bottom
                                //anchors.centerIn: coverRectangle.Center
                                color: Style.colorText
                        } // text
                        }


                } // Item



                MouseArea {
                    id: clickArea
                    anchors.fill: delegateItem
                    hoverEnabled: true
                    onEntered: {
                        delegateItem.scale = 1.5
                        delegateItem.z=1.0
                        if (model.descr.length >= model.title.length) {
                            delegateItemText.text = model.descr
                        }
                    }
                    onExited: {
                        delegateItem.z=0
                        delegateItem.scale = 1.0
                        if (model.title && model.title.length > 0){ delegateItemText.text = model.title;}
                    }
                    onClicked: {
                        if (model.url !== "") {
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
                Component.onDestruction: {

                    // TODO delete covers
                }


            } // item


        } //Component
    }

    QtObject {
        id: internal
        property int windowLeftMargin: 64
        property int windowRightMargin: 70

        property int folderWidth: 100
        property int folderHeight: 200
        property int horizontalFolderSpacing: 50
        property int verticalFolderSpacing: 100
    }
}
