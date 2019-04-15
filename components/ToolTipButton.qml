import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.12

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
