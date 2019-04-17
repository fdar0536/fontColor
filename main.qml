import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

import backend.FontDatabase 0.1
import backend.Clipboard 0.1
import backend.Utils 0.1
import "components"

ApplicationWindow
{
    id: root
    visible: true
    width: 750
    height: 300
    title: qsTr("fontColor for TCAX")
    
    property bool initialized: false
    
    FontDatabase
    {
        id: fontDatabase
    }
    
    Clipboard
    {
        id: clipboard
    }
    
    Utils
    {
        id: utils
    }
    
    MessageDialog
    {
        id: exitDialog
        title: root.title
        text: qsTr("Are you sure to exit?")
        icon: StandardIcon.Question
        standardButtons: StandardButton.Yes | StandardButton.No | StandardButton.Cancel
        
        onYes:
        {
            Qt.quit()
        }
    }
    
    onClosing:
    {
        close.accepted = false;
        exitDialog.open();
    }
    
    onAfterSynchronizing: function()
    {
        if (initialized === true)
        {
            return;
        }
        else
        {
            initialized = true;
        }
        
        if (fontDatabase.fontCount !== 0)
        {
            var i;
            var fontfamily;
            var file;
            for (i = 0; i < fontDatabase.fontCount; i++)
            {
                fontfamily = fontDatabase.getFontFamily(i);
                fontListModel.append
                ({
                     "family": fontfamily
                 });
            }
            
            fontfamily = fontDatabase.getFontFamily(0);
            titleText.text = fontfamily;
            fontComboBox.displayText = fontfamily;
            titleText.font.family = fontfamily;
            fontFileInfo.text = fontDatabase.getFontFileName(0);
            fontFamilyInfo.text = fontfamily;
            fontStyleInfo.text = fontDatabase.getFontStyle(0);
        }
        else
        {
            titleText.text = qsTr("Failed to load font database.");
            titleText.color = "#ff0000"
            fontComboBox.displayText = qsTr("Failed to load font database.");
            copyFontFileNameBtn.enabled = false
            copyFontFileBtn.enabled = false
            copyFontNameBtn.enabled = false
        }
    }
    
    Pane
    {
        x: 30
        y: 20
        width: parent.width - x * 2
        height: parent.height - y * 2
        
        Label
        {
            id: titleLabel
            height: titleText.height + 30
            anchors
            {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            
            TitleText
            {
                id: blank1
                text: " "
                anchors
                {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
            }
            
            TitleText
            {
                id: titleText
                anchors
                {
                    top: parent.top
                    bottom: parent.bottom
                    left: blank1.right
                    right: blank2.left
                }
            }
            
            TitleText
            {
                id: blank2
                text: " "
                anchors
                {
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
            }
            
            background: Rectangle
            {
                border.width: 1
            }
        } //end Label
        
        ComboBox
        {
            id: fontComboBox
            currentIndex: 0
            anchors
            {
                top: titleLabel.bottom
                left: parent.left
                right: parent.right
            }
            
            model: ListModel
            {
                id: fontListModel
            }
            
            delegate: ItemDelegate
            {
                text: model.family
            }
            
            onCurrentIndexChanged: function()
            {
                var res = model.get(currentIndex).family;
                displayText = res;
                titleText.text = res;
                titleText.font.family = res;
                fontFileInfo.text = fontDatabase.getFontFileName(currentIndex);
                fontFamilyInfo.text = res;
                fontStyleInfo.text = fontDatabase.getFontStyle(currentIndex);
            }
        } //end fontComboBox
        
        //file
        TitleText
        {
            id: fontFileTitle
            text: qsTr("Font file: ")
            pointSize: 15
            anchors
            {
                left: parent.left
                top: fontComboBox.bottom
                bottom: copyFontFileNameBtn.bottom
            }
        }
        
        TitleText
        {
            id: fontFileInfo
            pointSize: 15
            anchors
            {
                left: fontFileTitle.right
                right: copyFontFileNameBtn.left
                top: fontComboBox.bottom
                bottom: copyFontFileNameBtn.bottom
            }
        }
        
        ToolTipButton
        {
            id: copyFontFileNameBtn
            text: qsTr("Copy font's file name")
            tooltip: qsTr("Copy font's file name")
            anchors
            {
                top: fontComboBox.bottom
                right: fontPathText.left
            }
            
            onClicked: function()
            {
                clipboard.copyString(fontDatabase.getFontFileName(fontComboBox.currentIndex));
            }
        }
        
        Text
        {
            id: fontPathText
            text: " "
            anchors
            {
                top: copyFontFileBtn.top
                bottom: copyFontFileBtn.bottom
                right: copyFontFileBtn.left
            }
        }
        
        ToolTipButton
        {
            id: copyFontFileBtn
            text: qsTr("Copy File")
            tooltip: qsTr("Copy font's file")
            anchors
            {
                top: fontComboBox.bottom
                right: parent.right
            }
            
            onClicked:
            {
                clipboard.copyFile(fontDatabase.getFontFilePath(fontComboBox.currentIndex));
            }
        }
        
        //family
        TitleText
        {
            id: fontFamilytTitle
            text: qsTr("Font family: ")
            pointSize: 15
            anchors
            {
                left: parent.left
                top: copyFontFamilyBtn.top
                bottom: copyFontFamilyBtn.bottom
            }
        }
        
        TitleText
        {
            id: fontFamilyInfo
            pointSize: 15
            anchors
            {
                left: fontFamilytTitle.right
                right: copyFontFamilyBtn.left
                top: copyFontFamilyBtn.top
                bottom: copyFontFamilyBtn.bottom
            }
        }
        
        ToolTipButton
        {
            id: copyFontFamilyBtn
            text: qsTr("Copy font family")
            tooltip: qsTr("Copy font family")
            anchors
            {
                top: fontFileInfo.bottom
                right: parent.right
            }
            
            onClicked:
            {
                clipboard.copyString(fontDatabase.getFontFamily(fontComboBox.currentIndex));
            }
        }
        
        //style
        TitleText
        {
            id: fontStyleTitle
            text: qsTr("Font style: ")
            pointSize: 15
            anchors
            {
                left: parent.left
                top: styleRectangle.top
                bottom: styleRectangle.bottom
            }
        }
        
        TitleText
        {
            id: fontStyleInfo
            pointSize: 15
            anchors
            {
                left: fontStyleTitle.right
                right: styleRectangle.left
                top: styleRectangle.top
                bottom: styleRectangle.bottom
            }
        }
        
        Rectangle
        {
            id: styleRectangle
            width: 0
            height: copyFontFamilyBtn.height
            anchors
            {
                right: parent.right
                top: copyFontFamilyBtn.bottom
            }
        }
        
        //color
        ColorDialog
        {
            id: colorDialog
            title: "Please choose a color"
            visible: false;
            onAccepted:
            {
                previewColor.color = color;
                assColorText.text = utils.convertToAssColor(color);
            }
        }
        
        ToolTipButton
        {
            id: selectColorBtn
            text: qsTr("Select color")
            tooltip: qsTr("select color")
            anchors
            {
                left: parent.left
                top: fontStyleInfo.bottom
            }
            
            onClicked: colorDialog.open()
        }
        
        Text
        {
            id: selectPreviewText
            text: " "
            anchors
            {
                top: selectColorBtn.top
                bottom: selectColorBtn.bottom
                left: selectColorBtn.right
            }
        }
        
        Rectangle
        {
            id: previewColor
            height: selectColorBtn.height
            width: selectColorBtn.width
            anchors
            {
                left: selectPreviewText.right
                top: fontStyleInfo.bottom
            }
            
            border
            {
                width: 1
            }
            
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onEntered:
                {
                    previewColorTip.visible = true
                }
                
                onExited:
                {
                    previewColorTip.visible = false
                }
                
                onClicked: colorDialog.open()
            }
            
            ToolTip
            {
                id: previewColorTip
                visible: false
                text: qsTr("Preview color, or click to select color.")
                delay: 1000
                timeout: 3000
            }
        }
        
        Text
        {
            id: previewAssText
            text: " "
            anchors
            {
                top: selectColorBtn.top
                bottom: selectColorBtn.bottom
                left: previewColor.right
            }
        }
        
        TitleText
        {
            id: assColorText
            text: "FFFFFF"
            anchors
            {
                left: previewAssText.right
                top: selectColorBtn.top
                bottom: selectColorBtn.bottom
            }
            
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onEntered:
                {
                    assColorTextTip.visible = true
                }
                
                onExited:
                {
                    assColorTextTip.visible = false
                }
                
                onClicked: clipboard.copyString(assColorText.text)
            }
            
            ToolTip
            {
                id: assColorTextTip
                visible: false
                text: qsTr("Ass Color string, click to copy this")
                delay: 1000
                timeout: 3000
            }
        }
        
        Text
        {
            id: assCopyText
            text: " "
            anchors
            {
                top: selectColorBtn.top
                bottom: selectColorBtn.bottom
                left: assColorText.right
            }
        }
        
        ToolTipButton
        {
            text: qsTr("Copy ass color")
            tooltip: qsTr("Copy ass color")
            anchors
            {
                left: assCopyText.right
                top: fontStyleInfo.bottom
            }
            
            onClicked:
            {
                clipboard.copyString(assColorText.text);
            }
        }
        
        //about qt and exit
        ToolTipButton
        {
            id: exitBtn
            text: qsTr("Exit")
            tooltip: qsTr("Exit this program")
            anchors
            {
                top: fontStyleInfo.bottom
                right: parent.right
            }
            
            onClicked: exitDialog.open()
        }
        
        Text
        {
            id: about
            text: " "
            anchors
            {
                top: exitBtn.top
                bottom: exitBtn.bottom
                right: exitBtn.left
            }
        }
        
        ToolTipButton
        {
            text: qsTr("About Qt")
            tooltip: text
            anchors
            {
                top: fontStyleInfo.bottom
                right: about.left
            }
            
            onClicked: utils.aboutQt()
        }
    } //end Pane
} //end ApplicationWindow
