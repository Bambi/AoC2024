from subprocess import check_output

def run(day, inputf):
    with open(str(day) + "/" + inputf, "rb") as input:
        out = check_output(
            [ "./build/" + str(day) ],
            stdin=input,
        )
    return out.decode('UTF-8')

def test_1():
    assert run(1, "input") == "2086478 24941624\n"

def test_2():
    assert run(2, "input") == "356 413\n"

def test_3():
    assert run(3, "example1") == "161 161\n"
    assert run(3, "example2") == "161 48\n"
    assert run(3, "input") == "180233229 95411583\n"

def test_4():
    assert run(4, "input") == "2504 1923\n"

def test_5():
    assert run(5, "example1") == "143\n"
    assert run(5, "input") == "4814 5448\n"
