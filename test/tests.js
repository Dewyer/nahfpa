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
        script: "a + \\prod{10}{20}"
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
    },
    {
        title: "doc1",
        description: "Docs 1",
        script: "123456 3abc"
    },
    {
        title: "doc2",
        description: "Docs 2",
        script: "abc + def"
    },
    {
        title: "doc3",
        description: "Docs 3",
        script: "\\text{almafa, kifli, alma}"
    },
    {
        title: "doc4",
        description: "Docs 4",
        script: "\\frac{123}{a + b}"
    },
    {
        title: "doc5",
        description: "Docs 5",
        script: "\\sqrt{a + 23}"
    },
    {
        title: "doc6",
        description: "Docs 6",
        script: "{alma}^{a + b}"
    },
    {
        title: "doc7",
        description: "Docs 7",
        script: "{korte}_{a + b}"
    },
    {
        title: "doc8",
        description: "Docs 8",
        script: "\\index{a}{10}{20}"
    },
    {
        title: "doc9",
        description: "Docs 9",
        script: "\\sum{i}"
    },
    {
        title: "doc10",
        description: "Docs 10",
        script: "\\prod{i}"
    },
    {
        title: "doc11",
        description: "Docs 11",
        script: "\\Omega"
    },
    {
        title: "doc12",
        description: "Docs 12",
        script: "\\lim{n \\to \\inf}n + 1"
    },
    {
        title: "doc13",
        description: "Docs 13",
        script: "\\#{Ez egy komment} 123"
    },
    {
        title: "dstring",
        description: "DString bench",
        script: "(\\frac{123}{345}) + \\beta^{200 + 100}" // new : ALLOC CC 68, max: 64
    }
];
module.exports = {tests};