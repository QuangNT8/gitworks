import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.2
import "../commons"

Flickable
{
    contentWidth: width
    contentHeight: container.height
    bottomMargin: 10

    ColumnLayout
    {
        id: container
        spacing: 0
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        SectionTitle
        {
            text: 'Menu'
            icon: ''
        }

        ListModel
        {
            id: recipeList
            ListElement{name: 'Cafe Đá Xay Caramel'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cafe Đá Xay Cacao'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Matcha Đá Xay'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Latte'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cappuccino'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Ice Caramel Macchiato'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Americano'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Espresso'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Bạc Xỉu Đá'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cà Phê Sữa'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Bạc Xỉu'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cafe Đen'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cold Brew Bưởi Hồng'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cold Brew Original'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cold Brew Xoài'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
            ListElement{name: 'Cold Brew Đào'; image: 'https://www.sciencenews.org/sites/default/files/main/articles/100315_coffee_opener_NEW_0.jpg'}
        }

        GridLayout
        {
            columns: 2
            columnSpacing: 8
            rowSpacing: 8
            visible: menuRepeater.count != 0
            Repeater
            {
                id: menuRepeater
                model: Home.recipes
                delegate: CategoryItem
                {
                }
            }
        }
    }
}
