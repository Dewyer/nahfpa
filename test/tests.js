const tests = [
    {
        title: "frac",
        description: "Frac test.",
        script: "\\frac{1}{2} = 2",
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
    {
        title: "bracket1",
        description: "Bracket 1",
        script: "(100 + 20)"  
    },
    {
        title: "bracket2",
        description: "Bracket 2",
        script: "2 + (\\frac{123}{456})"  
    },
    {
        title: "square",
        description: "Bracket 2",
        script: "1022 + [\\frac{123}{456}] + 42"
    },
    {
        title: "identity2",
        description: "Identity 2",
        script: "e^z = \\lim{n \\to \\inf} (1+\\frac{z}{n})^n"
    }
];
module.exports = {tests};