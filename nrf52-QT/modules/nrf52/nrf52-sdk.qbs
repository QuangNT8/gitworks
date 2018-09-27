import qbs

StaticLibrary
{
    name: "nrf52-sdk"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}


    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths: product.includePaths
    }

    cpp.includePaths: product.includePaths

    Group
    {
        name: "nrf_COMMON"
        files:
        [

        ]
    }
}
