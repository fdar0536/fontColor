import QtQuick

Text
{
    id: root
    property alias pointSize: root.font.pointSize
    font.pointSize: 25
    smooth: true
    verticalAlignment: Text.AlignVCenter
}
