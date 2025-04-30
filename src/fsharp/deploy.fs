open System

let logDeployment pkg =
    printfn "Deploying %s via F# automation..." pkg

logDeployment "depmii-example"
