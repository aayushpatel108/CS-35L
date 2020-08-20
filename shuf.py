#!/usr/bin/python3

import random, sys
from optparse import OptionParser

class shuf:
    def __init__(self, lines):
        random.shuffle(lines)
        self.lines = lines

    def chooseline(self):
        return random.choice(self.lines)

def input_handler(options, args):
    try:
        options.num_lines = int(options.num_lines)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.num_lines))
    if options.num_lines < 0:
        parser.error("negative count: {0}".
                     format(options.num_lines))
    elif (options.input_range):
        if (len(args) != 0):
            parser.error("Extra argument: {0}".format(args[0]))
        try:
            lo, hi = options.input_range.split("-")
            if int(lo) > int(hi):
                parser.error("invalid input range: '{0}'"
                .format(options.input_range))
            lines = [str(i) + '\n' for i in list(range(int(lo), int(hi) + 1))]
        except:
            parser.error("invalid input range: '{0}'"
            .format(options.input_range))
    elif (len(args) == 0 or (len(args) == 1 and args[0] == "-")):
        lines = sys.stdin.readlines()
    else:
        input_file_name = args[0]
        try:
            with open(input_file_name, 'r') as input_file:
                lines = input_file.readlines()
        except FileNotFoundError: 
            parser.error('File does not exist')
    if (not options.repeat):
        num_lines = min(options.num_lines, len(lines))
    else:
        num_lines = options.num_lines
    return lines, num_lines

def parse_arguments():
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-i", "--input-range",
                      action="store", dest="input_range",
                      help="""Act as if input came from a file containing the 
                      range of unsigned decimal integers lo-hi, 
                      one per line.""")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="""Repeat output values, that is, select with 
                      replacement. With this option the output is not a 
                      permutation of the input; instead, each output line is 
                      randomly chosen from all the inputs. This option is 
                      typically combined with --head-count; if --head-count 
                      is not given, shuf repeats indefinitely.""") 
    parser.add_option("-n", "--head-count", 
                      action="store", type = "int", dest="num_lines", 
                      default=sys.maxsize,
                      help="""Output at most count lines. 
                      By default, all input lines are output.""")                                                                                               
    return parser 

if __name__ == "__main__":
    version_msg = "shuf.py"
    usage_msg = """python3 shuf.py [file]]… [option]...
                   python3 shuf.py [option]… [file]...
                   python3 shuf.py -i [option] …
                   python3 shuf.py -n [option]… [arg]…
                   python3 shuf.py -r -n [option]… [arg]…
                   """
    parser = parse_arguments()
    options, args = parser.parse_args(sys.argv[1:])
    lines, num_lines  = input_handler(options,args)
    try:
        generator = shuf(lines)
        for i in range(num_lines):
            if (options.repeat):
                sys.stdout.write(generator.chooseline())
            else:
                sys.stdout.write(generator.lines[i])
    except IOError as error:
        parser.error("I/O error: {1}".format(error.args))