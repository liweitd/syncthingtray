import QtQuick
import QtQuick.Controls

import Main

Dialog {
    Material.primary: Material.LightBlue
    Material.accent: Material.LightBlue
    parent: Overlay.overlay
    anchors.centerIn: Overlay.overlay
    popupType: App.nativePopups ? Popup.Native : Popup.Item
    width: Math.min(popupType === Popup.Item ? parent.width - 20 : implicitWidth, 800)
    standardButtons: Dialog.Yes | Dialog.No
    modal: true
}
