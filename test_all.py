from subprocess import check_output

def run(day):
    with open(str(day) + "/input", "rb") as input:
        out = check_output(
            [ "./build/" + str(day) ],
            stdin=input,
        )
    return out.decode('UTF-8')

def test_1():
    assert run(1) == '''Distance: 2086478
Similitary score: 24941624
'''

def test_2():
    assert run(2) == "356 413\n"
