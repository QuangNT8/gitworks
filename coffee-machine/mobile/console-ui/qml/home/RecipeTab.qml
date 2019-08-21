import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.3
import'../commons'

BasePage
{
    id: recipeTab
    RowLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: '#fff'
            radius: 3
            Flickable
            {
                anchors.fill: parent
                contentWidth: width
                contentHeight: recipeGrid.height

                GridLayout
                {
                    id: recipeGrid
                    width: parent.width
                    columns: 4
                    columnSpacing: 5
                    rowSpacing: 5

                    Rectangle
                    {
                        Layout.preferredWidth: (parent.width-(recipeGrid.columns-1)*recipeGrid.columnSpacing)/recipeGrid.columns
                        Layout.preferredHeight: width
                        color: '#ccc'
                        border.color: '#ccc'
                        radius: 3
                        Label
                        {
                            anchors.centerIn: parent
                            text: Flaticon.get('add')
                            font.family: 'Flaticon'
                            font.pixelSize: 60
                            color: '#666'
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked:
                            {
                                recipeDetailDialog.recipeValue = undefined
                                recipeDetailDialog.recipeImage.source = ''
                                recipeDetailDialog.fileImage = ''
                                recipeDetailDialog.open()
                            }
                        }
                    }

                    Repeater
                    {
                        id: recipeList
                        model: Console.recipes
                        delegate: Rectangle
                        {
                            Layout.preferredWidth: (parent.width-(recipeGrid.columns-1)*recipeGrid.columnSpacing)/recipeGrid.columns
                            Layout.preferredHeight: width
                            color: '#ccc'
                            radius: 3
                            Image
                            {
                                id: img
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectCrop
                                asynchronous: true
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
                                            radius: 5
                                        }
                                    }
                                }
                                Component.onCompleted:
                                {
                                    var tmp = model.image.trim();
                                    if (tmp.startsWith('http')) img.source = tmp;
                                    else if(tmp.length > 0)
                                    {
                                        var f = file.getFilePath(model.image);
                                        if (f.length > 0)
                                        {
                                            img.source = 'file://'+f
                                        }
                                        else
                                        {
                                            var op = Engine.createOperation("machine.DownloadFile")
                                            op.finished.connect(fileDownloaded)
                                            op.name = model.image;
                                            op.fire();
                                        }
                                    }
                                    else
                                    {
                                        img.source ='qrc:/images/default_recipe.jpg'
                                        //sendLogError(model.name +' don\'t have image');
                                    }

                                }

                                function fileDownloaded(code)
                                {
                                    if (code === 0)
                                    {
                                        img.source = 'file://' + file.location() +'/'+ model.image
                                    }
                                    else
                                    {
                                        img.source ='qrc:/images/default_recipe.jpg'
                                        //sendLogError(model.name +' don\'t have image')
                                    }
                                }
                            }
                            Rectangle
                            {
                                anchors.bottom: parent.bottom
                                width: parent.width
                                height: 45
                                color: '#444'
                                opacity: 0.9
                                radius: parent.radius
                                Label
                                {
                                    anchors.centerIn: parent
                                    width: parent.width - 20
                                    text: model.name
                                    font.family: 'Flaticon'
                                    color: '#eee'
                                    font.pixelSize: 16
                                    font.bold: true
                                    maximumLineCount: 2
                                    wrapMode: Label.Wrap
                                    elide: "ElideRight"
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                Rectangle
                                {
                                    width: parent.width
                                    height: parent.radius
                                    color: parent.color
                                }
                            }
                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked:
                                {
                                    recipeDetailDialog.recipeValue = model
                                    recipeDetailDialog.recipeImage.source = img.source.toString().startsWith("qrc:") ? '' :  img.source
                                    recipeDetailDialog.fileImage = model.image
                                    recipeDetailDialog.open()
                                }
                            }
                        }
                    }
                    Item
                    {
                        Layout.fillWidth: true
                        visible: recipeList.count<5
                    }
                }
            }
        }
        RecipeDetailDialog
        {
            id: recipeDetailDialog
        }
    }
}

