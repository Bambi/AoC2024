from subprocess import check_output

def d1():
    with open("1/input", "rb") as input:
        out = check_output(
            [ "./build/1" ],
            stdin=input,
        )
    return out.decode('UTF-8')

def test_1():
    assert d1() == '''Distance: 2086478
Similitary score: 24941624
'''
