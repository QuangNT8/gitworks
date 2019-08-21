import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import'../commons'

Item
{
    Layout.fillWidth: true
    Layout.preferredHeight: 45
    property var currentVolume
    property var currentMin
    property var currentMax
    property var currentID : Console.getLiquidID(model)
    RowLayout
    {
        anchors.fill: parent
        //anchors.leftMargin: 10
        //anchors.rightMargin: 10
        Label
        {
            Layout.leftMargin: 10
            Layout.fillWidth: true
            text: model.name
            color: '#444'
            font.pixelSize: 15
        }
        Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 1
            color: '#ddd'
        }

        TextInput
        {
            clip: true
            Layout.preferredWidth: 60
            Layout.fillHeight: true
            Layout.rightMargin: 5
            text: recipeValue !== undefined  ? Console.getIngredient(model, recipeValue):0
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: "AlignRight"
            color: text === "0" ? '#666' : Material.color(Material.Pink)
            font.bold: true
            selectionColor: Material.color(Material.Pink)
            validator: RegExpValidator{regExp: /^[0-9]*$/}
            selectByMouse: true
            onTextChanged: currentVolume =  text
            onTextEdited: dirty = true //FIXME: performance drop maybe?
            onFocusChanged:
            {
                if (focus) selectAll()
                if (text === "") text = "0"
            }
            MouseArea
            {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: Qt.IBeamCursor
            }
        }

//        Rectangle
//        {
//            Layout.fillHeight: true
//            Layout.preferredWidth: 1
//            color: '#ddd'
//        }

//        TextInput
//        {
//            clip: true
//            Layout.preferredWidth: 60
//            Layout.fillHeight: true
//            verticalAlignment: TextInput.AlignVCenter
//            horizontalAlignment: "AlignRight"
//            text: recipeValue !== undefined  ? Console.getIngredient(model, recipeValue,1):0
//            validator: RegExpValidator{regExp: /^[0-9]*$/}
//            selectByMouse: true
//            /*TODO: selectAll on focus*/
//            onTextChanged:
//            {
//                currentMin =  text
//            }
//            onFocusChanged: if (text === "") text = "0"

//        }
//        Rectangle
//        {
//            Layout.fillHeight: true
//            Layout.preferredWidth: 1
//            color: '#ddd'
//        }

//        TextInput
//        {
//            clip: true
//            Layout.preferredWidth: 60
//            Layout.fillHeight: true
//            verticalAlignment: TextInput.AlignVCenter
//            horizontalAlignment: "AlignRight"
//            rightPadding: 8
//            text: recipeValue !== undefined  ? Console.getIngredient(model, recipeValue,2):0
//            validator: RegExpValidator{regExp: /^[0-9]*$/}
//            selectByMouse: true
//            /*TODO: selectAll on focus*/
//            onTextChanged:
//            {
//                currentMax =  text
//            }
//            onFocusChanged: if (text === "") text = "0"
//        }
    }
    Rectangle
    {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: '#ddd'
    }
}
