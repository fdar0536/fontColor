import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

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

        onSortingDone:
        {
            if (fontDatabase.fontCount !== 0)
            {
                ui_enabler(true);
                updateHook(0, 0);
                busyIndicator.running = false;
            }
            else
            {
                loading_failed();
            }
        }
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
        buttons: MessageDialog.Yes | MessageDialog.No | MessageDialog.Cancel

        onAccepted:
        {
            Qt.quit()
        }
    }

    BusyIndicator
    {
        id: busyIndicator
        anchors.centerIn: mainPane
        width: 50
        height: 50
        z: 50
        running: true
    }

    function ui_enabler(input)
    {
        if (input === false)
        {
            titleText.text = qsTr("Loading...");
            fontFamilyInfo.text = ""
            fontFileInfo.text = "";
            fontStyleInfo.text = "";
        }

        copyFontFileNameBtn.enabled = input;
        copyFontFileBtn.enabled = input;
        copyFontFamilyBtn.enabled = input;
        fontComboBoxAsc.enabled = input;
        fontComboBoxDesc.enabled = input;
    }

    function loading_failed()
    {
        titleText.text = qsTr("Failed to load font database.");
        titleText.color = "#ff0000";
        fontComboBox.displayText = qsTr("Failed to load font database.");
        busyIndicator.running = false;
    }

    function updateHook(input1, input2)
    {
        var index = fontDatabase.getCurrentFontIndex(input1);
        var fontfamily = fontDatabase.getFontFamily(index);
        titleText.text = fontfamily;
        titleText.font.family = fontfamily;
        fontFamilyInfo.text = fontfamily;

        var style = fontDatabase.getFontStyle(index);
        fontfamily += " (" + style + ")";
        fontComboBox.displayText = fontfamily;
        fontFileInfo.text = fontDatabase.getFontFileName(index);
        fontStyleInfo.text = style;

        if (input2 === 0)
        {
            fontComboBox.currentIndex = 0;
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

        ui_enabler(false);
        if (fontDatabase.fontCount !== 0)
        {
            fontDatabase.font_asc_sort();
        }
        else
        {
            loading_failed();
        }
    }

    ColorDialog
    {
        id: colorDialog
        title: "Please choose a color"
        visible: false;
        onAccepted:
        {
            previewColor.color = selectedColor;
            assColorText.text = utils.convertToAssColor(selectedColor);
        }
    }

    Pane
    {
        id: mainPane
        x: 30
        y: 20
        width: parent.width - x * 2
        height: parent.height - y * 2

        ColumnLayout
        {
            spacing: 10

            TitleText
            {
                id: titleText
            }

            RowLayout
            {
                ComboBox
                {
                    id: fontComboBox
                    currentIndex: 0
                    Layout.alignment: Qt.AlignLeft
                    implicitContentWidthPolicy: ComboBox.WidestText

                    model: fontDatabase

                    delegate: ItemDelegate
                    {
                        text: fontDatabase.getCurrentFontFamily(index)
                    }

                    onCurrentIndexChanged: function()
                    {
                        if (fontDatabase.fontCount === 0)
                        {
                            return;
                        }

                        updateHook(currentIndex, -1);
                    }
                } //end fontComboBox

                ButtonGroup
                {
                    id: radioButtonGroup
                    Layout.alignment: Qt.AlignRight
                }

                RadioButton
                {
                    id: fontComboBoxAsc
                    Layout.alignment: Qt.AlignRight
                    text: qsTr("Ascending order")
                    ButtonGroup.group: radioButtonGroup
                    checked: true

                    onCheckedChanged:
                    {
                        ui_enabler(false);
                        if (fontDatabase.fontCount !== 0)
                        {
                            if (checked === false)
                            {
                                fontDatabase.font_desc_sort();
                            }
                            else
                            {
                                fontDatabase.font_asc_sort();
                            }

                            busyIndicator.running = true;
                        }
                        else
                        {
                            loading_failed();
                        }
                    }
                }

                RadioButton
                {
                    id: fontComboBoxDesc
                    text: qsTr("Descending order")
                    Layout.alignment: Qt.AlignRight
                    ButtonGroup.group: radioButtonGroup
                }
            } // end RowLayout

            // font file
            RowLayout
            {
                TitleText
                {
                    id: fontFileTitle
                    text: qsTr("Font file: ")
                    pointSize: 15
                    Layout.alignment: Qt.AlignLeft
                }

                TitleText
                {
                    id: fontFileInfo
                    pointSize: 15
                    Layout.alignment: Qt.AlignLeft
                }

                ToolTipButton
                {
                    id: copyFontFileNameBtn
                    text: qsTr("Copy font's file name")
                    tooltip: qsTr("Copy font's file name")
                    Layout.alignment: Qt.AlignRight

                    onClicked: function()
                    {
                        clipboard.copyString(fontDatabase.getFontFileName
                                             (fontDatabase.getCurrentFontIndex
                                              (fontComboBox.currentIndex)));
                    }
                }

                ToolTipButton
                {
                    id: copyFontFileBtn
                    text: qsTr("Copy File")
                    tooltip: qsTr("Copy font's file")
                    Layout.alignment: Qt.AlignRight

                    onClicked:
                    {
                        clipboard.copyFile(fontDatabase.getFontFilePath
                                           (fontDatabase.getCurrentFontIndex
                                            (fontComboBox.currentIndex)));
                    }
                }
            } // RowLayout

            // font family
            RowLayout
            {
                TitleText
                {
                    id: fontFamilytTitle
                    text: qsTr("Font family: ")
                    pointSize: 15
                    Layout.alignment: Qt.AlignLeft
                }

                TitleText
                {
                    id: fontFamilyInfo
                    pointSize: 15
                    Layout.alignment: Qt.AlignLeft
                }

                ToolTipButton
                {
                    id: copyFontFamilyBtn
                    text: qsTr("Copy font family")
                    tooltip: qsTr("Copy font family")
                    Layout.alignment: Qt.AlignRight

                    onClicked:
                    {
                        clipboard.copyString(fontDatabase.getFontFamily
                                             (fontDatabase.getCurrentFontIndex
                                              (fontComboBox.currentIndex)));
                    }
                }
            } // RowLayout

            // font style
            RowLayout
            {
                TitleText
                {
                    id: fontStyleTitle
                    text: qsTr("Font style: ")
                    pointSize: 15
                }

                TitleText
                {
                    id: fontStyleInfo
                    pointSize: 15
                }
            } // RowLayout

            RowLayout
            {
                spacing: 10

                //color
                ToolTipButton
                {
                    id: selectColorBtn
                    text: qsTr("Select color")
                    tooltip: qsTr("select color")
                    Layout.alignment: Qt.AlignLeft

                    onClicked: colorDialog.open()
                }

                Rectangle
                {
                    id: previewColor
                    height: selectColorBtn.height
                    width: selectColorBtn.width
                    Layout.alignment: Qt.AlignLeft

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

                TitleText
                {
                    id: assColorText
                    text: "FFFFFF"
                    Layout.alignment: Qt.AlignLeft

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

                ToolTipButton
                {
                    text: qsTr("Copy ass color")
                    tooltip: qsTr("Copy ass color")
                    Layout.alignment: Qt.AlignLeft

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
                    Layout.alignment: Qt.AlignRight

                    onClicked: exitDialog.open()
                }

                ToolTipButton
                {
                    text: qsTr("About Qt")
                    tooltip: text
                    Layout.alignment: Qt.AlignRight

                    onClicked: utils.aboutQt()
                }
            } // RowLayout
        } // end ColumnLayout
    } //end Pane
} //end ApplicationWindow
