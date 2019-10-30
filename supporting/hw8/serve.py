#!/usr/bin/python3
# #!/usr/local/bin/python3 # for laptop
# #!/env/bin/python3  # for gcloud
"""
Simple HTTP server in python, that invokes an executable to handle requests.
Usage::
    ./serve.py [exe_name] [port]
where exe_name is the name of the compiled executable, and port is the desired
    port number.
Send a POST request::
    curl -d "3ok" http://localhost:25199
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
from subprocess import *
import sys, socket, os

gcloud = -1

if socket.gethostname().startswith('STO'):  # personal machine
  get_port = lambda : int(os.environ.get("PORT")) if os.environ.get("PORT") is not None else 25199
else:
  try: # see if running on link machines
    # sys.path.insert(0, '/usr/local/cs/cs251')
    # sys.path.append('/home/users/olaf/pub/cs251')
    sys.path.append('/usr/local/cs/cs251/port')
    from port import get_port
    #get_port = lambda : os.environ.get("PORT") if os.environ.get("PORT") is not None else 35200
  except ModuleNotFoundError: # otherwise assume Google Cloud or personal machine 
    get_port = lambda : int(os.environ.get("PORT")) if os.environ.get("PORT") is not None else 8080
    print(get_port())
    gcloud = 1

exe_name = "a.out"
exe_mtime = -1
message_id = {}
yaml_files_name = "app.files"
mem_file_name = "app.mem"
yaml_mtime = -1
separator = '\n---\n'

def has_top_key(key, yaml):
  return yaml.startswith(key) or -1 != yaml.find('\n' + key)

def get_top_key(key, yaml):
  if yaml.startswith(key):
    res = yaml[len(key):]
  else:
    pos = yaml.find('\n' + key)
    if -1 == pos:
      return False
    res = yaml[pos + len(key) + 1:]
  # trim off after newline
  pos2 = res.find('\n')
  if -1 == pos2:
    return res
  res = res[:pos2]
  return res

def load_binary(filename):
    with open(filename, 'rb') as fh:
        return fh.read()

class S(BaseHTTPRequestHandler):
  def _set_headers(self, content):
      self.send_response(200)
      self.send_header('Content-type', content)
      self.send_header('Access-Control-Allow-Origin', '*')
      self.end_headers()

  def use_exe(self, first_name, yaml_part, incoming_data):
    global exe_name, exe_mtime, message_id, separator
    new_mtime = os.path.getmtime(exe_name)
    is_poll = False
    just_recompiled = False
    if new_mtime > exe_mtime:  # restart!
      message_id[first_name] = 0
      incoming_data = "- message_id: " + str(message_id[first_name]) + "\n"
      incoming_data += "- first_name: " + first_name + separator
      incoming_data = incoming_data.encode()
      sys.stderr.write("recompiled")
      just_recompiled = True
    else:
      # sys.stderr.write(" first chars of yaml_part are " + yaml_part[:20] + ' ')
      if has_top_key('- initialize: ', yaml_part):  # initialization
        message_id[first_name] = 0
        incoming_data = "- message_id: " + str(message_id[first_name]) + "\n"
        incoming_data += "- first_name: " + first_name + separator
        incoming_data = incoming_data.encode()
        sys.stderr.write('init_client') 
      elif has_top_key('- event_info:', yaml_part):   # user interaction
        message_id[first_name] += 1
        incoming_data = ("- message_id: " + str(message_id[first_name]) + "\n").encode() + incoming_data
        sys.stderr.write('event')
      else: # just a poll
        # is_poll = True
        incoming_data = ("- message_id: " + str(message_id[first_name]) + " poll\n").encode() + incoming_data
    if is_poll:
      outgoing_data = 'same_exe'
      outgoing_data = bytes(outgoing_data, "utf-8")
    else:
      # (stdout, stderr) = Popen('./' + exe_name, stdin=PIPE, stdout=PIPE, stderr=PIPE).communicate(bytes(incoming_data, "utf-8"))
      vverbose = False
      if vverbose:
        sys.stderr.write(" running with incoming len:\n")
        sys.stderr.buffer.write(incoming_data)
        sys.stderr.write("\n")
      # (stdout_, stderr_) = Popen('./' + exe_name, stdin=PIPE, stdout=PIPE, stderr=PIPE).communicate(incoming_data)
      proc = Popen('./' + exe_name, stdin=PIPE, stdout=PIPE, stderr=PIPE)
      try:
          (stdout_, stderr_) = proc.communicate(incoming_data, timeout=2)
      except TimeoutExpired:
          proc.kill()
          (stdout_, stderr_) = proc.communicate()
          stderr_ = b'timeout expired:  your code may have an infinite loop!\n' + stderr_
      # print(stderr, "types", type(stdout), type(stderr))  # bytes!
      if 'str' == type(stdout_):
        stdout_ = bytes(stdout_, 'utf-8')
        stderr_ = bytes(stderr_, 'utf-8')
      if vverbose:
        sys.stderr.write("\n STDOUT ")
        sys.stderr.buffer.write(stdout_)
        sys.stderr.write("\n")
      keyword = b'React Native'
      if stdout_.startswith(keyword):
        if vverbose: sys.stderr.write(" keyword ")
        stdout_ = stdout_[len(keyword):]
      elif b'same_exe' == stdout_:
        if vverbose: sys.stderr.write(" was_poll ")
      elif b'timeout' == stdout_:
        if vverbose: sys.stderr.write(" timeout ")
      else:
        if vverbose: sys.stderr.write(" no_keyword ")
        if vverbose: sys.stderr.buffer.write(stdout_)
        if os.path.isfile(yaml_files_name):
          if just_recompiled:
            sys.stderr.write(" sending static ")
          stdout_ = self.use_static_yaml_inner()
        else:
          stdout_ = b''
      outgoing_data = stdout_
      if len(stderr_) > 0:
        outgoing_data = b'- stderr: ' + stderr_ + bytes(chr(0), "utf-8") + outgoing_data
        # outgoing_data = bytes(outgoing_data, "utf-8")  # orig!
        print(' ' + stderr_.decode(), file=sys.stderr)
    self._set_headers('text/html')
    self.wfile.write(outgoing_data)
    exe_mtime = new_mtime
  
  def use_static_yaml_inner(self):
    # sys.stderr.write(" use_static_yaml_inner\n")
    global separator, yaml_mtime
    new_mtime = os.path.getmtime(yaml_files_name)
    new_mtime2 = os.path.getmtime(mem_file_name)
    if new_mtime2 > new_mtime:
      new_mtime = new_mtime2
    if new_mtime > yaml_mtime:
      sys.stderr.write("sending static yaml")
    outgoing_data = ''
    with open(yaml_files_name, 'r') as fh:
      for line in fh:
        line = line.lstrip()
        if line[0] != '#':
          line = line.rstrip()
          with open(line, 'r') as fh2:
            outgoing_data += '\n' + fh2.read()
      outgoing_data += separator
      mem_data = b''
      with open(mem_file_name, 'rb') as fh3:
        mem_data += fh3.read()
      while len(mem_data) < 10000:  # fill in the rest with null bytes
        mem_data += b'\0'
      outgoing_data = outgoing_data.encode('utf8') + mem_data
      # print('outgoing_data is as follows:\n', outgoing_data)
      yaml_mtime = new_mtime
    return outgoing_data

  def use_static_yaml(self):
    outgoing_data = self.use_static_yaml_inner()
    self._set_headers('text/html')
    self.wfile.write(outgoing_data)

  def do_GET(self):
      if self.path.endswith(".png"):
          self._set_headers('image/png')
          self.wfile.write(load_binary(self.path[self.path.rfind("/")+1:]))
      
  def do_POST(self):
      content_length = int(self.headers['Content-Length'])
      incoming_data = self.rfile.read(content_length)
      # parse the incoming data, to look for some key parts
      if False and len(incoming_data) > 50:
        print("incoming", ord(incoming_data[30]), ord(incoming_data[31]), file=sys.stderr)
      global exe_name, separator
      sep_pos = incoming_data.find(separator.encode())
      if -1 == sep_pos:
        yaml_part = ''
      else:
        yaml_part = incoming_data[:sep_pos].decode() + '\n'
      first_name = get_top_key('- first_name: ', yaml_part)
      # print("first_name is " + first_name, file=sys.stderr)

      # does the executable exist?
      if os.path.isfile(exe_name):
        # sys.stderr.write(" has_exe ")
        self.use_exe(first_name, yaml_part, incoming_data)
      elif os.path.isfile(yaml_files_name):
        # sys.stderr.write(" no_exe ")
        self.use_static_yaml()

  def log_message(self, format, *args):
      sys.stderr.write(".")
      sys.stderr.flush()
        
def run(server_class=HTTPServer, handler_class=S, port=25199):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    hostname = socket.gethostname()
    cwd = os.getcwd()
    if len(cwd) > 25:
      cwd = '...' + cwd[len(cwd)-25:]
    print('Starting httpd at', (hostname + ':' + str(port)), 'with', exe_name, 'in', cwd)
    httpd.serve_forever()

if __name__ == "__main__":
  from sys import argv

  if len(argv) == 3:
    exe_name = argv[1]
    run(port=int(argv[2]))
  elif len(argv) == 2:
    port = get_port()
    exe_name = argv[1]
    run(port=port)
  elif len(argv) == 1:
    port = get_port()
    run(port=port)
  else:
    print("Usage: ./serve.py [exe_name] [port]")
