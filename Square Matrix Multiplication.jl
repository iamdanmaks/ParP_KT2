
using Distributed
addprocs(4)

@everywhere using SharedArrays

function multiply_matrices(matr1, matr2, matr3)
    dim = size(matr1, 2)
    
    @sync @distributed for i in 1:dim
        for j in 1:dim
            temp = 0
            for k in 1:dim
                temp += matr1[i, k] * matr2[k, j]
            end
            matr3[i, j] = temp
        end
    end
end

function multiply_matrices_non_parallel(matr1, matr2, matr3)
    dim = size(matr1, 2)
    
    for i in 1:dim+1
        for j in 1:dim+1
            temp = 0
            for k in 1:dim+1
                temp += matr1[i, k] * matr2[k, j]
            end
            matr3[i, j] = temp
        end
    end
end

RESULTS = Dict(100 => Dict("par" => 0.0, "non_par" => 0.0, "iters" => 1000), 
    200 => Dict("par" => 0.0, "non_par" => 0.0, "iters" => 1000), 500 => Dict("par" => 0.0, "non_par" => 0.0, "iters" => 150), 
    750 => Dict("par" => 0.0, "non_par" => 0.0, "iters" => 50))

for (k, v) in RESULTS
    mt1 = rand(-100:100, k, k)
    mt2 = rand(-100:100, k, k)
    
    let i = 0
        while i < v["iters"]
            mt3 = convert(SharedArray, zeros(Int64, k, k))
            mt4 = convert(SharedArray, zeros(Int64, k, k))
            
            v["par"] = @elapsed multiply_matrices(mt1, mt2, mt3)
            v["non_par"] = @elapsed multiply_matrices_non_parallel(mt1, mt2, mt4)
            
            i += 1
        end
    end
    
    v["par"] /= v["iters"]
    v["non_par"] /= v["iters"]
    
    println("$k was done")    
end

RESULTS
