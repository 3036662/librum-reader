import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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

      MTitle {
          id: pageTitle
          Layout.topMargin: 44
          titleText: "OPDS  catalogs"
          descriptionText: "Find books in public OPDS libraries"
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
                  id: grid
                  anchors.fill: parent
                  cellWidth: internal.folderWidth + internal.horizontalFolderSpacing
                  cellHeight: internal.folderHeight + internal.verticalFolderSpacing
                  model:contactModel
                  delegate: contactDelegate

              }
      }

      Component {
              id: contactDelegate
              Item {
                  width: grid.cellWidth; height: grid.cellHeight
                  Column {
                      anchors.fill: parent
                      Image {
                          width: parent.width
                          height: parent.width
                          fillMode :Image.Stretch
                          source: Icons.opdsFolder;
                          anchors.horizontalCenter: parent.horizontalCenter }
                      Text {
                          text: name;
                          anchors.horizontalCenter: parent.horizontalCenter
                          color: Style.colorText
                      }
                  }
              }
        }

  }

  ListModel {
     id: contactModel
      ListElement {
          name: "Jim Williams"
      }
      ListElement {
          name: "John Brown"
      }
      ListElement {
          name: "Bill Smyth"
      }
      ListElement {
          name: "Sam Wise"
      }
  }


      QtObject {
          id: internal
          property int windowLeftMargin: 64
          property int windowRightMargin: 70

          property int folderWidth: 80
          property int folderHeight: 160
          property int horizontalFolderSpacing: 64
          property int verticalFolderSpacing: 48
      }


}
