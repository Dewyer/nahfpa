
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
        title: "lim",
        description: "Lim offseted",
        script: "b + \\lim{10}"
    },
];
module.exports = {tests};