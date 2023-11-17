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

              onClicked: ;// FIXME importFilesDialog.open()
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
                  width: opdsGrid.cellWidth; height: opdsGrid.cellHeight
                  Column {
                      anchors.fill: parent
                      Image {
                          antialiasing :true;
                          width: parent.width
                          height: parent.width
                          fillMode :Image.Stretch
                          source: Icons.opdsFolder;
                          anchors.horizontalCenter: parent.horizontalCenter
                          Image{
                                id:nodeThumbnail
                                //asynchronous:true
                                cache : true
                                anchors.centerIn: parent
                                width: parent.width*0.9
                                fillMode: Image.PreserveAspectFit
                               // source:  "https://doc.qt.io/style/qt-logo-documentation.svg"
                                source:model.imgDataReady === false ?  "" : "image://opds_image_provider/"+model.imageUrl ;
                                visible: model.imgDataReady;
                                sourceSize.height: 160
                                sourceSize.width: 80
                          }

                          AnimatedImage{
                                id:loadingAnimation
                                anchors.centerIn: parent
                                width: parent.width/2
                                playing: false
                                visible: false
                                fillMode:Image.PreserveAspectFit
                                source: Icons.loadingAnimation
                          }

                      }
                      Text {
                          id: delegateItemText
                          text: model.title;
                          maximumLineCount:3
                          horizontalAlignment: Text.AlignHCenter
                          wrapMode: Text.Wrap
                          width: parent.width
                          padding: 5
                          anchors.horizontalCenter: parent.horizontalCenter
                          color: Style.colorText
                      }
                  }

                  MouseArea{
                      id:clickArea
                      anchors.fill: parent
                      hoverEnabled: true
                      onEntered:{
                           delegateItem.scale = 1.1
                          if (model.descr.length >= model.title.length){
                            delegateItemText.text=model.descr;
                          }
                      }
                      onExited: {
                          delegateItem.scale=1.0
                          delegateItemText.text=model.title;
                      }
                      onClicked: {
                            if (model.url !== ""){
                            loadingAnimation.playing = true;
                            loadingAnimation.visible = true;
                            OpdsController.loadRootLib(model.url);
                             }
                      }
                  } // MouseArea
                  // load images
                 Component.onCompleted:{
                            OpdsController.getNodeImage(model.id);
                  }
                  Component.onDestruction: {
                         // TODO delete covers
                  }

              } // item
      }

  }


      QtObject {
          id: internal
          property int windowLeftMargin: 64
          property int windowRightMargin: 70

          property int folderWidth: 80
          property int folderHeight: 160
          property int horizontalFolderSpacing: 84
          property int verticalFolderSpacing: 68
      }


}
