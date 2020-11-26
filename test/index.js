const { exec } = require("child_process");
const path = require('path');
const fs = require('fs');
const Handlebars = require("handlebars");
const { tests } = require("./tests");
const open = require('open');


const root = path.resolve("../")
const binary = `${root}\\cmake-build-debug\\nahfpa.exe`

function execPromise(command)
{
    return new Promise((res,rej)=>{
        exec(command, (error, stdout, stderr) =>
        {
            res({ error, stdout, stderr });
        });
    })
}

async function compileString(name, code)
{
    fs.writeFileSync("./artifacts/temp/inp.math", code);
    const outName = `./artifacts/${name}.svg`;
    let res = await execPromise(`${binary} -i ./artifacts/temp/inp.math -o ${outName} --color no`);
    if (res.error === null)
    {
        return {
            ...res,
            code: fs.readFileSync(outName, {encoding: "utf-8"})
        }
    }
    return {...res, code: null};
}

function extractSizeFromStdout(stdout)
{
    try{
        let wh = stdout.split('Box model dimensions: ')[1].split('\r')[0].split(',').map(el => el.trim()).map(el => el.split(': ')[1]);
        return {
            width: wh[0],
            height: wh[1]
        }
    } catch (ex)
    {
        console.log(ex);
        return {width: 0, height: 0};
    }
}

async function runTests()
{
    let ranTests = [];
    let cc = 0;
    for (let tt of tests)
    {
        const compRes = await compileString(tt.title, tt.script);
        const size = extractSizeFromStdout(compRes.stdout);
        ranTests.push({
            no: `${cc+1}/${tests.length}`,
            ...tt,
            result: compRes.code ? `data:image/svg+xml;utf8,${compRes.code}` : "No output generated",
            testResultSuccess: !compRes.error ===  !tt.shouldFail,
            ...size,
            out: compRes.stdout || ''
        });
        cc++;
    }

    return ranTests;
}

async function main()
{
    const tests = await runTests();

    const template = Handlebars.compile(fs.readFileSync("./test_template.hbs", {encoding:"utf-8"}));
    fs.writeFileSync("test.html", template({
        tests: tests
    }));

    await open("./test.html", { app: 'firefox' });
}

main().then(()=>console.log("Test suite quitting.")).catch((er)=>{throw er;});