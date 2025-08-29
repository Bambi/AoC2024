from subprocess import check_output
from pytest import mark

def run(day, inputf):
    ddir = dexe = str(day)
    print(ddir, dexe)
    if day > 25:
        ddir = ddir[:-1]
    with open(ddir + "/" + inputf, "rb") as input:
        out = check_output(
            [ "./build/" + dexe ],
            stdin=input,
        )
    return out.decode('UTF-8')

@mark.parametrize("day, file, result",
[
    (1, "ex",       "11 31\n"),  
    (2, "ex",       "2 4\n"),  
    (3, "example1", "161 161\n"),
    (3, "example2", "161 48\n"),
    (4, "example1", "18 9\n"),
    (5, "example1", "143 123\n"),
    (6, "ex1",      "41 6\n"),
    (7, "ex",       "3749 11387\n"),
    (8, "ex",       "14\n"),
    (82, "ex",      "34\n"),
    (9, "ex1",      "60 132\n"),
    (9, "ex2",      "1928 2858\n"),
    (10,"ex1",      "1 16\n"),
    (10,"ex2",      "2 2\n"),
    (10,"ex3",      "4 13\n"),
    (10,"ex4",      "3 6\n"),
    (10,"ex5",      "36 81\n"),
    (10,"ex6",      "1 3\n"),
    (10,"ex7",      "4 13\n"),
    (10,"ex8",      "2 227\n"),
    (11,"ex1",      "55312\n"),
    (112,"ex1",     "65601038650482\n"),
    (12,"ex1",      "140 80\n"),
    (12,"ex2",      "772 436\n"),
    (12,"ex3",      "1930 1206\n"),
    (12,"ex4",      "692 236\n"),
    (12,"ex5",      "1184 368\n"),
    (13,"ex1",      "480 875318608908\n"),
    (14,"ex1",      "12\n"),
    (15,"ex1",      "2028\n"),
    (15,"ex2",      "10092\n"),
])
def test_day(day, file, result):
    assert run(day, file) == result
