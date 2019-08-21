import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3
import '../commons'

Popup
{
    property bool dirty: false
    property var recipeValue
    property var fileImage
    property alias recipeImage: img
    id: detailDialog
    anchors.centerIn: parent
    implicitWidth: /*750*/ parent.width - 50
    implicitHeight: /*650*/ parent.height - 50
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose
    function updateRecipe()
    {
        var name = nameInput.text.trim();
        var description = descriptionInput.text.trim();
        if (name.length === 0)
        {
            showPageError('Please enter recipe name', 3000)
            sendLogError('Please enter recipe name')
        }
        else if (description.length === 0)
        {
            showPageError('Please enter recipe description', 3000)
            sendLogError('Please enter recipe description')
        }
        else
        {
            //Ingredient
            var list_ingredients = new Array;
            for (var i = 0; i < repeatLiquid.count; i++)
            {
                var val = repeatLiquid.itemAt(i).currentVolume;
                var n_val = parseInt(val);
                if(n_val > 0)
                    list_ingredients.push(
                                {
                                    liquidid: repeatLiquid.itemAt(i).currentID,
                                    volume: n_val
                                });

            }

            if(list_ingredients.length === 0)
            {
                showPageError('Please enter ingredient value', 3000)
                sendLogError('Please enter ingredient value')
                return;
            }

            showLoading()
            var op = Engine.createOperation("machine.UpdateRecipe")
            op.finished.connect(function(code){done(code); if(code === 0){ recipeValue = undefined; detailDialog.close();}})
            op.ingredients = list_ingredients;
            op.recipe = (recipeValue === undefined ? null : recipeValue); //null <=> add
            op.name = name;
            op.desc = description;
            op.image = fileImage;
            op.fire();
            timeOut.start();
        }
    }
    function deleteRecipe()
    {
        if(recipeValue === undefined ) return;
        showLoading()
        var op = Engine.createOperation("machine.RemoveRecipe")
        op.finished.connect(function(code){done(code); if(code === 0){ recipeValue = undefined; detailDialog.close();}})
        op.recipe = recipeValue;
        op.fire();
        timeOut.start();
    }
    function checkOrder()
    {
        var vol = makeVolumeInput.text.trim();
        if(vol.length === 0 || parseInt(vol) <= 0)
        {
            showPageError('Please enter volume greater than 0 ', 3000)
            sendLogError('Please enter volume greater than 0 ')
            makeDrinkDialog.open()
        }
        else
        {
            var list_ingredients = new Array;
            for (var i = 0; i < repeatLiquid.count; i++)
            {
                var val = repeatLiquid.itemAt(i).currentVolume;
                if(parseInt(val) > 0)
                {
                    list_ingredients.push(
                                {
                                    liquidid: repeatLiquid.itemAt(i).currentID,
                                    volume: val
                                });
                }
            }
            if(list_ingredients.length === 0)
            {
                showPageError('Please enter ingredient value', 3000)
                sendLogError('Please enter ingredient value')
                return;
            }
            showLoading()
            var op = Engine.createOperation("machine.CheckOrder")
            op.finished.connect(

                        function(code)
                        {
                            timeOut.stop()
                            hideLoading();
                            if(code === 0)
                            {
                                showLoading()
                                var op = Engine.createOperation("machine.MakeDrink")
                                op.finished.connect(done)
                                op.orderid = Console.orderid;
                                op.fire();
                                timeOut.start();
                            }
                            else if(code ===1) //door bussy
                            {
                                showPageError('Door is busy', 3000)
                                sendLogError('Door is busy')
                            }
                            else if(code ===2) //ingredients not enough
                            {
                                showPageError('Ingredients not enough', 3000)
                                sendLogError('Ingredients not enough')
                            }
                            else
                            {
                                showPageError('Cannot process your order', 3000)
                                sendLogError('Cannot process your order')
                            }

                        })
            op.door = leftDoor.checked ? 1 : 2;
            op.volume = vol;
            op.ingredients = list_ingredients;
            op.fire();
            timeOut.start();
        }
    }
    function makeDrink()
    {
        var vol = makeVolumeInput.text.trim();
        if(vol.length === 0 || parseInt(vol) <= 0)
        {
            showPageError('Please enter volume greater than 0 ', 3000)
            sendLogError('Please enter volume greater than 0 ')
            makeDrinkDialog.open()
        }
        else
        {
            var list_ingredients = new Array;
            for (var i = 0; i < repeatLiquid.count; i++)
            {
                var val = repeatLiquid.itemAt(i).currentVolume;
                if(parseInt(val) > 0)
                {
                    list_ingredients.push(
                                {
                                    liquidid: repeatLiquid.itemAt(i).currentID,
                                    volume: val
                                });
                }
            }
            if(list_ingredients.length === 0)
            {
                showPageError('Please enter ingredient value', 3000)
                sendLogError('Please enter ingredient value')
                return;
            }
            showLoading()
            var op = Engine.createOperation("machine.MakeDrink")
            op.finished.connect(
                function(code)
                {
                    timeOut.stop()
                    hideLoading();
//                    BUSY = 1;
//                    LIQUID_NOT_ENOUGH = 2;
//                    IN_DEBUG_MODE = 3;
//                    MACHINE_CRASHED = 4;
//                    MOTOR_INVALID = 5;
//                    NEURAL_NETWORK_INVALID = 6;
                    if(code === 1)
                    {
                        showPageError('Door is busy', 3000)
                        sendLogError('Door is busy')
                    }
                    else if(code === 2)
                    {
                        showPageError('Motor invalid', 3000)
                        sendLogError('Motor invalid')
                    }
                    else if(code ===3)
                    {
                        showPageError('In debug mode', 3000)
                        sendLogError('In debug mode')
                    }
                    else if(code === 4)
                    {
                        showPageError('Machine crashed', 3000)
                        sendLogError('Machine crashed')
                    }
                    else if(code === 5)
                    {
                        showPageError('Motor invalid', 3000)
                        sendLogError('Motor network invalid')
                    }
                    else if(code === 6)
                    {
                        showPageError('Neural network invalid', 3000)
                        sendLogError('Neural network invalid')
                    }

                })
            op.door = leftDoor.checked ? 1 : 2;
            op.volume = vol;
            op.ingredients = list_ingredients;
            op.fire();
            timeOut.start();
        }
    }
    function done(code)
    {
        timeOut.stop()
        hideLoading();
        if (code !== 0) showPageError("System error. Please try again later", 3000)
    }
    Timer
    {
        id: timeOut
        interval: 15000; running: false; repeat: false
        onTriggered:
        {
            showPageError("The connection to the server failed.")
            hideLoading();
        }
    }
    contentItem: ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 0
        RowLayout
        {
            //layoutDirection: "RightToLeft"
            spacing: 10
            Item
            {
                id: generalInfo
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                ColumnLayout
                {
                    anchors.fill: parent
                    spacing: 0
                    SectionTitle
                    {
                        Layout.rightMargin: 10
                        text: 'Name'
                        icon: ''
                    }
                    RectangularInput
                    {
                        id: nameInput
                        Layout.fillWidth: true
                        text: recipeValue === undefined ? '' : recipeValue.name
                        maximumLength: 50
                    }

                    SectionTitle
                    {
                        text: 'Description'
                        icon: ''
                    }
                    Rectangle
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        radius: 3
                        clip: true
                        color: 'transparent'
                        border.color: '#ddd'
                        TextInput
                        {
                            id: descriptionInput
                            anchors.fill: parent
                            anchors.margins: 5
                            text: recipeValue === undefined ? '' : recipeValue.description
                            wrapMode: Text.WordWrap
                            color: '#666'
                            font.pixelSize: 15
                            selectByMouse: true
                            MouseArea
                            {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: Qt.IBeamCursor
                            }
                        }


                    }
                    SectionTitle
                    {
                        text: 'Image'
                        icon: Flaticon.get('folder')
                        Layout.rightMargin: 4
                        onClicked: imageFileDialog.open();
                    }

                    Rectangle
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: width
                        color: '#ccc'
                        border.color: '#ccc'
                        radius: 3
                        Image
                        {
                            id: img
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            //source: recipeValue === undefined ? '' : 'file://'+Console.getLocation() +'/' + recipeValue.image
                            sourceSize: Qt.size(parent.width, parent.height)
                            layer.enabled: true
                            layer.effect: OpacityMask
                            {
                                maskSource: Item
                                {
                                    width: img.width
                                    height: img.height
                                    Rectangle
                                    {
                                        anchors.centerIn: parent
                                        width: parent.width
                                        height: parent.height
                                        radius: 3
                                    }
                                }
                            }

                        }
                    }
                }
            }

            ColumnLayout
            {
                id: detailInfo
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                SectionTitle
                {
                    text: 'Ingredients'
                    icon: ''
                }

                Rectangle
                {
                    clip: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    border.color: '#ccc'
                    Flickable
                    {
                        anchors.fill: parent
                        contentWidth: width
                        contentHeight: liquidList.height
                        clip: true
                        ColumnLayout
                        {
                            id: liquidList
                            width: parent.width
                            spacing: 0
                            Rectangle
                            {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 45
                                color: '#666'
                                RowLayout
                                {
                                    anchors.fill: parent
                                    Label
                                    {
                                        Layout.fillWidth: true
                                        Layout.leftMargin: 10
                                        text: 'Name'
                                        color: '#eee'
                                        font.bold: true
                                    }
                                    Rectangle
                                    {
                                        Layout.fillHeight: true
                                        Layout.preferredWidth: 1
                                        color: '#ddd'
                                    }

                                    Label
                                    {
                                        Layout.preferredWidth: 60
                                        Layout.rightMargin: 5
                                        text: 'Volume\n(ml)'
                                        color: '#eee'
                                        font.bold: true
                                        horizontalAlignment: Label.AlignHCenter

                                    }
                                    //                                    Rectangle
                                    //                                    {
                                    //                                        Layout.fillHeight: true
                                    //                                        Layout.preferredWidth: 1
                                    //                                        color: '#ddd'
                                    //                                    }

                                    //                                    Label
                                    //                                    {
                                    //                                        Layout.preferredWidth: 60
                                    //                                        text: 'Min\n(ml)'
                                    //                                        color: '#eee'
                                    //                                        font.bold: true
                                    //                                        horizontalAlignment: Label.AlignHCenter

                                    //                                    }
                                    //                                    Rectangle
                                    //                                    {
                                    //                                        Layout.fillHeight: true
                                    //                                        Layout.preferredWidth: 1
                                    //                                        color: '#ddd'
                                    //                                    }

                                    //                                    Label
                                    //                                    {
                                    //                                        Layout.preferredWidth: 60
                                    //                                        text: 'Max\n(ml)'
                                    //                                        color: '#eee'
                                    //                                        font.bold: true
                                    //                                        horizontalAlignment: Label.AlignHCenter
                                    //                                    }

                                }
                            }
                            Repeater
                            {
                                id: repeatLiquid
                                model: Console.liquids
                                delegate: RecipeDetailItem{}
                            }
                        }
                    }
                }
            }
        }

        RowLayout
        {
            id: buttonSection
            spacing: 15
            Button
            {
                Layout.preferredWidth: 100
                text: 'Delete'
                flat: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                visible: recipeValue !== undefined
                onClicked:
                {
                    dirty = false
                    deleteRecipeConfirm.open()
                }
            }
            Item
            {
                Layout.fillWidth: true
            }
            Button
            {
                Layout.preferredWidth: 100
                text: dirty? 'Discard' : 'Cancel'
                flat: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                onClicked:
                {
                    dirty? cancelConfirm.open() : detailDialog.close();
                }
            }
            Button
            {
                Layout.preferredWidth: 100
                Material.accent: Material.color(Material.Blue)
                text: 'Save'
                flat: true
                highlighted: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                onClicked:
                {
                    dirty = false
                    updateRecipe()
                }
            }
            Button
            {
                Layout.preferredWidth: 100
                Material.accent: Material.color(Material.Brown)
                text: 'Make'
                flat: true
                highlighted: true
                hoverEnabled: true
                font.bold: hovered ? true : false
                onClicked: makeDrinkDialog.open()
            }
        }
    }
    BaseDialog
    {
        id: cancelConfirm
        parent: recipeTab
        title: 'You may have unsaved changes'
        message: 'This will discard all changes you made. Are you sure?'
        onAccepted:
        {
            dirty = false
            detailDialog.close()
        }
    }
    BaseDialog
    {
        id: deleteRecipeConfirm
        parent: recipeTab
        title: 'Delete this recipe'
        message: 'This deletion can not be undone. Are you sure?'
        onAccepted: deleteRecipe()
    }
    FileDialog
    {
        id: imageFileDialog
        title: "Please choose a image file"
        folder: shortcuts.home
        nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
        onAccepted:
        {
            img.source = imageFileDialog.fileUrl;
            fileImage = imageFileDialog.fileUrl;
        }
    }
    BaseDialog
    {
        id: makeDrinkDialog
        width: parent.width*0.6
        title: 'Make drink with current recipe'
        height: contentHeight + 150
        Component.onCompleted: makeVolumeInput.forceActiveFocus()
        contentItem: ColumnLayout
        {
            anchors.centerIn: parent
            width: parent.width
            spacing: 10
            RowLayout
            {
                spacing: 5
                Layout.fillWidth: true
                Label
                {
                    Layout.preferredWidth: 200
                    text: 'Volume to make (ml)'
                    font.pixelSize: 15
                    color: '#333'
                }
                TextField
                {
                    id: makeVolumeInput
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 40
                    text: "200"
                    topPadding: 5
                    bottomPadding: 5
                    rightPadding: 10
                    leftPadding: 10
                    selectByMouse: true
                    validator: RegExpValidator{regExp: /^[0-9]*$/}
                    inputMethodHints: Qt.ImhDigitsOnly
                    background: Rectangle{border.color: '#aaa'; radius: 3}
                    onFocusChanged: if (focus) selectAll()
                }
            }
            RowLayout
            {
                Layout.preferredHeight: 40
                spacing: 10
                Label
                {
                    Layout.preferredWidth: 200
                    text: 'Select door'
                    font.pixelSize: 15
                    color: '#333'
                }
                RadioButton
                {
                    id:leftDoor
                    text: 'Left'
                    checked: true
                }
                RadioButton
                {
                    text: 'Right'
                }
            }
        }
        onAccepted: makeDrink()
    }
}


