
const tests = [
    {
        title: "frac",
        description: "Frac test.",
        script:"\\frac{1}{2} = 2",
        shouldFail: false
    },
    {
        title: "sum",
        description: "Sum offseted",
        script: "a + \\sum{10}"
    },
    {
        title: "prod",
        description: "prod offseted",
        script: "a + \\prod{10}"
    },
    {
        title: "lim",
        description: "Lim offseted",
        script: "b + \\lim{10}"
    },
    {
        title: "lim",
        description: "Lim wo arg",
        script: "belta + \\lim"
    },
    {
        title: "eip",
        description: "Lim offseted",
        script: "e^{i \\pi} = -1 + 100^\\Omega"
    },
    {
        title: "ind",
        description: "Index",
        script: "\\index{\\frac{1}{2}}{\\inf}{\\omega}"
    },
    {
        title: "sqrt",
        description: "Sqrt offseted",
        script: "100 + \\sqrt{\\frac{1}{asdasd}}"
    },
    {
        title: "text",
        description: "Text",
        script: "200 + \\sqrt{\\text{\\frac{keksz} lolzer}}"
    },
];
module.exports = {tests};