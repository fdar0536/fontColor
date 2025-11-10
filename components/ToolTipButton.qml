import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal

Button
{
    id: root
    property alias tooltip: btnToolTip.text

    hoverEnabled: true

    ToolTip
    {
        id: btnToolTip
        visible: root.hovered
        delay: 1000
        timeout: 2000
    }
}
