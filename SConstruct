vars = Variables()
vars.Add('BACKTRACK_SORT_HEURISTIC', 'Set to 1 to use a sorting heuristic in backtracking.', 0)
vars.Add('OPTIMIZE', 'Set to 0, 1, 2 or 3 depending on optimization level.', 0)
env = Environment(variables = vars,
                  CPPDEFINES={'BACKTRACK_SORT_HEURISTIC' : '${BACKTRACK_SORT_HEURISTIC}'},
                  CCFLAGS='-O${OPTIMIZE}')

Help(vars.GenerateHelpText(env))

env.Program("sudoku",Glob("*.c"))
