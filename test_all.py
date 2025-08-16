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
    (1, "input",    "2086478 24941624\n"),  
    (2, "input",    "356 413\n"),  
    (3, "example1", "161 161\n"),
    (3, "example2", "161 48\n"),
    (3, "input",    "180233229 95411583\n"),
    (4, "input",    "2504 1923\n"),
    (5, "example1", "143 123\n"),
    (5, "input",    "4814 5448\n"),
    (6, "ex1",      "41 6\n"),
    (6, "input",    "5461 1836\n"),
    (7, "ex",       "3749 11387\n"),
    (7, "input",    "538191549061 34612812972206\n"),
    (8, "ex",       "14\n"),
    (8, "input",    "367\n"),
    (82, "ex",      "34\n"),
    (82, "input",   "1285\n"),
])
def test_day(day, file, result):
    assert run(day, file) == result
