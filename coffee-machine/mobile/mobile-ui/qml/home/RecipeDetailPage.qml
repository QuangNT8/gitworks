import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import '../commons'

BasePage
{
    id: basePage
    property bool isFavorite: false
    property var recipe
    property var imgSource
    isHomePage: true
    headerTitle: recipe.name

    function reloadRecipe()
    {
        for (var i = 0; i < liquidRepeater.count; i++)
        {
            liquidRepeater.itemAt(i).slider.value = liquidRepeater.itemAt(i).slider.originalValue
            liquidRepeater.itemAt(i).slider.lastValue = liquidRepeater.itemAt(i).slider.value
        }
    }

    function checkOrder()
    {
        var list_ingredients = new Array;
        for (var i = 0; i < liquidRepeater.count; i++)
        {
            var val = liquidRepeater.itemAt(i).slider.value;
            if(parseInt(val) > 0)
            {
                list_ingredients.push
                ({
                    liquidid: liquidRepeater.itemAt(i).currentID,
                    volume: val
                });
            }
        }
        if(list_ingredients.length === 0)
        {
            showPageError('Please enter ingredient value', 3000)
            return;
        }
        var volume = 0;
        if (cup200.checked) volume = 200;
        else if (cup275.checked) volume = 275;
        else if (cup330.checked) volume = 330;

        showPage('qrc:/home/SelectMachinePage.qml', {recipe: recipe, ingredients: list_ingredients, volume: volume})
    }

    footer: Item
    {
        height: 65
        Rectangle
        {
            anchors.fill: parent
            color: '#ccc'
        }
        Button
        {
            anchors.centerIn: parent
            Material.accent: Material.Green
            width: parent.width - 20
            height: parent.height - 10
            text: 'Order'
            font.pixelSize: 20
            font.bold: true
            highlighted: true
            onClicked: checkOrder()
        }
    }
    Flickable
    {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: width
        contentHeight: container.height

        ColumnLayout
        {
            id: container
            width: parent.width
            spacing: 5

            Item
            {
                visible: false
                Layout.topMargin: -10
                Layout.leftMargin: -10
                Layout.rightMargin: -10
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                RowLayout
                {
                    anchors.fill: parent
                    RoundButton
                    {
                        Layout.preferredWidth: 50
                        text: Flaticon.get('back')
                        font.family: 'Flaticon'
                        flat: true
                    }
                    Label
                    {
                        Layout.fillWidth: true
                        color: '#333'
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        elide: "ElideRight"
                    }
                    RoundButton
                    {
                        Layout.preferredWidth: 50
                        text: Flaticon.get('star')
                        font.family: 'Flaticon'
                        flat: true
                    }
                }
            }

            RecipePageHeader
            {
                Layout.topMargin: -10
                Layout.leftMargin: -10
                Layout.rightMargin: -10
                radius: 0
            }

            SectionTitle
            {
                text: 'Cup size'
                icon: ''
            }
            BaseItem
            {
                Layout.fillWidth: true
                Layout.preferredHeight: selectVolumeLayout.height + bottomPadding*2
                RowLayout
                {
                    id: selectVolumeLayout
                    anchors.centerIn: parent
                    width: parent.width
                    spacing: 15
                    RadioButton
                    {
                        id: cup200
                        Layout.alignment: Qt.AlignHCenter
                        text: '200ml'
                        checked: true
                    }
                    RadioButton
                    {
                        id: cup275
                        Layout.alignment: Qt.AlignHCenter
                        text: '275ml'
                    }
                    RadioButton
                    {
                        id: cup330
                        Layout.alignment: Qt.AlignHCenter
                        text: '330ml'
                    }
                }
            }
            SectionTitle
            {
                text: 'Ingredients'
                icon: Flaticon.get('ac-auto')
                onClicked: reloadRecipe()
            }

            BaseItem
            {
                id: customTasteItem
                Layout.fillWidth: true
                Layout.preferredHeight: customLiquidLayout.height + bottomPadding*2
                ColumnLayout
                {
                    id: customLiquidLayout
                    anchors.centerIn: parent
                    width: parent.width
                    spacing: 5
                    Repeater
                    {
                        id: liquidRepeater
                        model: Home.liquids
                        delegate: ColumnLayout
                        {
                            spacing: 0
                            property alias slider: slider
                            property var currentID : Home.getLiquidID(model)
                            visible: Home.getIngredient(model,recipe) > 0
                            Label
                            {
                                Layout.fillWidth: true
                                text: model.name
                                color: '#444'
                                font.pixelSize: 15
                                font.bold: true
                            }
                            Slider
                            {
                                id: slider
                                property var originalValue: Home.getIngredient(model,recipe)
                                property var lastValue: originalValue
                                Layout.fillWidth: true
                                from: 0
                                to: 100
                                value: Home.getIngredient(model,recipe)
                                stepSize: 1
                                onPressedChanged:
                                {
                                    if (!pressed)
                                    {
                                        var isLastLiquid = true;
                                        for (var i = 0; i < liquidRepeater.count; i++)
                                        {
                                            if(liquidRepeater.itemAt(i).slider.value >0 && i !== index)
                                            {
                                                isLastLiquid = false;
                                                break;
                                            }
                                        }
                                        if (isLastLiquid) value = lastValue;
                                        else lastValue = value;
                                    }
                                }
                            }
                        }
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: 'Can not get recipe of this drink'
                        color: '#333'
                        horizontalAlignment: Text.AlignHCenter
                        visible: liquidRepeater.count === 0
                    }
                }
            }
        }
    }
}
