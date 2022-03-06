import sys
import json as js
from haralyzer import HarParser, HarPage


def main(argv):
	sitename = str(argv[1])
	parsed = js.load(open(sitename+'.har'))['log']

	print len(parsed['entries']), len(parsed['pages'])
	print parsed['entries'][0].keys()
	print parsed['pages'][0].keys()







if __name__ == "__main__":
    main(sys.argv)

