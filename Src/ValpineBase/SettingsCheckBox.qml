import QtQuick 2.5
import QtQuick.Controls 1.4

import ValpineBase 1.0

SettingsBaseControl {
	id: root
	height: 30

	function assignFromSettingsValue(value) {
		checkBox.checked = value;
	}

	function provideSettingsValue() {
		return checkBox.checked;
	}

	CheckBox {
		id: checkBox
		text: ""
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.top: parent.top
		anchors.left: titleText.right
		anchors.margins: 6

		onClicked: {
			root.userChangedValue();
		}
	}
}
