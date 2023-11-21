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

        RowLayout { // toolbar
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
        }  // toolbar
		
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
                delegate: MOpdsNode{
                    MouseArea {
                        id: clickArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            parent.scale = 1.1
//                            if (model.descr.length >= model.title.length) {
//                                bookName.text = model.descr
//                            }
                        }
                        onExited: {
                            parent.scale = 1.0
//                            if (model.title && model.title.length > 0){ bookName.text = model.title;}
                        }

                        onClicked: {
                            if (model.url !== "") {
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
        } // Pane
    } // ColumnLayout

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
