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
])
def test_day(day, file, result):
    assert run(day, file) == result
