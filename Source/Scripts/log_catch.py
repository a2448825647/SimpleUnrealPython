import sys


class Redirect:
    content = ""

    def write(self, logStr):
        self.content += logStr

    def get(self):
        return self.content

    def clear(self):
        self.content = ''


_redirect = Redirect()


def redirectLog():
    sys.stdout = _redirect
    sys.stderr = _redirect


def getLog():
    return _redirect.get()


def clearLog():
    return _redirect.clear()
