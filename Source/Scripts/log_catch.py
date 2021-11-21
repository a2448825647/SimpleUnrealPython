import sys
import uelog


class RedirectNormal:
    content = ""
    def write(self, logStr):
        uelog.print(logStr)
        self.content+=logStr
    def getlog(self):
        return self.content
    def testLog(self):
        uelog.print('TestPythonLog')
        
class RedirectError:
    def write(self, logStr):
        uelog.error(logStr)


g_redirect_normal = RedirectNormal()
g_redirect_error = RedirectError()


def redirectLog():
    sys.stdout = g_redirect_normal
    sys.stderr = g_redirect_error
    
def getNormalLog():
    return g_redirect_normal.getlog()
    
def testNormalLog():
    g_redirect_normal.testLog()
