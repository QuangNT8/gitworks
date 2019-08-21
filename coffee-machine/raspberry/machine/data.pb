liquids:
[
    {
        id:  1
        name: "Water"
    },
    {
        id:  2
        name: "Coffee"
    },
    {
        id:  3
        name: "Sugar"
    },
    {
        id:  4
        name: "Milk"
    },
    {
        id:  5
        name: "Matcha"
    },
    {
        id:  6
        name: "Cacoa"
    },
    {
        id:  7
        name: "Coconut Cream"
    },
    {
        id:  8
        name: "Orange Siroop"
    },
    {
        id: 9
        name: "Strawberry Siroop"
    },
    {
        id: 10
        name: "Vanilla"
    },
    {
        id: 11
        name: "Lemon Juice"
    }
]

recipes:
[
    {
        id: 1
        name: "Cappuccino"
        image: "cappuccino.jpg"
        description: "An espresso-based coffee drink that originated in Italy, and is traditionally prepared with steamed milk foam"
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            },
            {
                liquidId: 3
                volume: 10
            },
            {
                liquidId: 5
                volume: 10
            },
            {
                liquidId: 7
                volume: 10
            }
        ]
    },
    {
        id: 2
        name: "Espresso"
        image: "espresso.jpg"
        description: "A coffee of Italian origin, brewed by forcing a small amount of nearly boiling water under pressure (expressing) through finely ground coffee beans"
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            },
            {
                liquidId: 3
                volume: 10
            },
            {
                liquidId: 5
                volume: 10
            },
            {
                liquidId: 7
                volume: 10
            }
        ]
    },
    {
        id: 3
        name: "Latte"
        image: "latte.jpg"
        description: "A "light & soft" coffee drink made with espresso and steamed milk."
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            }
        ]
    },
    {
        id: 4
        name: "Americano"
        image: "americano.jpg"
        description: "A type of coffee drink prepared by diluting an espresso with hot water, giving it a similar strength to, but different flavor from, traditionally brewed coffee"
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            }
        ]
    },
    {
        id: 6
        name: "Fresh Juice"
        image: "freshjuice.jpg"
        description: "Fresh your day with a "mash up" from lemon, orange and strawberry. Ready to cooling your summer!"
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            }
        ]
    },
    {
        id: 7
        name: "Coffee Matcha"
        image: "coffeematcha.jpg"
        description: "Strange taste from Japan. Why don't you try it now?"
        ingredients:
        [
            {
                liquidId: 1
                volume: 10
            },
            {
                liquidId: 2
                volume: 10
            }
        ]
    }
]
