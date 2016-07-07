
# Author: xyh
# Date: 2015.10.12

import sys, os, stat
import argparse
from optparse import OptionParser
import shutil

# xxtea for python
# -------------------------------------------------------------------------------------------------------------------------------
import struct 

_DELTA = 0x9E3779B9  

def _long2str(v, w):  
    n = (len(v) - 1) << 2  
    if w:  
        m = v[-1]  
        if (m < n - 3) or (m > n): return ''  
        n = m  
    s = struct.pack('<%iL' % len(v), *v)  
    return s[0:n] if w else s  
  
def _str2long(s, w):  
    n = len(s)  
    m = (4 - (n & 3) & 3) + n  
    s = s.ljust(m, "\0")  
    v = list(struct.unpack('<%iL' % (m >> 2), s))  
    if w: v.append(n)  
    return v  
  
def encrypt(str, key):  
    if str == '': return str  
    v = _str2long(str, True)  
    k = _str2long(key.ljust(16, "\0"), False)  
    n = len(v) - 1  
    z = v[n]  
    y = v[0]  
    sum = 0  
    q = 6 + 52 // (n + 1)  
    while q > 0:  
        sum = (sum + _DELTA) & 0xffffffff  
        e = sum >> 2 & 3  
        for p in xrange(n):  
            y = v[p + 1]  
            v[p] = (v[p] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff  
            z = v[p]  
        y = v[0]  
        v[n] = (v[n] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[n & 3 ^ e] ^ z))) & 0xffffffff  
        z = v[n]  
        q -= 1  
    return _long2str(v, False)  
  
def decrypt(str, key):  
    if str == '': return str  
    v = _str2long(str, False)  
    k = _str2long(key.ljust(16, "\0"), False)  
    n = len(v) - 1  
    z = v[n]  
    y = v[0]  
    q = 6 + 52 // (n + 1)  
    sum = (q * _DELTA) & 0xffffffff  
    while (sum != 0):  
        e = sum >> 2 & 3
        for p in xrange(n, 0, -1):  
            z = v[p - 1]
            v[p] = (v[p] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff  
            y = v[p]
        z = v[n]
        v[0] = (v[0] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[0 & 3 ^ e] ^ z))) & 0xffffffff  
        y = v[0]
        sum = (sum - _DELTA) & 0xffffffff
    return _long2str(v, True)
# -------------------------------------------------------------------------------------------------------------------------------

def cur_file_dir():
    path = sys.path[0]
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)

curdir = cur_file_dir()
dir_updateFiles = os.path.join(curdir, '../ResourcesFactory/encryptfiles/updateFiles')
dir_updateFilesc = os.path.join(curdir, '../Resources/updateFiles')

dir_res = os.path.join(curdir, '../ResourcesFactory/encryptfiles/res')
dir_resc = os.path.join(curdir, '../Resources/res')

__encryptkey__ = 'winturn2015'
__encryptsign__ = 'xSfjgrtuFFsj'

# do not encrypt json and atlas because of compressed package size
#ValidFileExt = ['.png','.json','.atlas','.dat','.jpg']
ValidFileExt = ['.png','.dat','.jpg']


class ResEncrypt(object):
    def run(self):
        print '----------- begin encrypt updateFiles -----------'
        self.run_encrypt(dir_updateFiles, dir_updateFilesc)
        print '----------- succeed encrypt updateFiles -----------'

        print '----------- begin encrypt res -----------'
        self.run_encrypt(dir_res, dir_resc)
        print '----------- succeed encrypt res -----------'


    def run_encrypt(self, dir_res, dir_resc):
        # parse argv
        argv = '-s %s -d %s -k %s -b %s' % (dir_res, dir_resc, __encryptkey__, __encryptsign__)
        argv = argv.split(' ')
        self.parse_args(argv)

        # make dst dic
        try:
            os.makedirs(self._dst_dir)
        except OSError:
            if os.path.exists(self._dst_dir) == False:
                print("Error: cannot create folder in " + self._dst_dir)

        # deep iterate the res directory
        for res_dir in self._res_dir_arr:
            self._current_res_dir = res_dir
            self._res_files[self._current_res_dir] = []
            self.deep_iterate_dir(res_dir)

        # handle files, copy and encrypt
        self.handle_all_res_files()


    def normalize_path_in_list(self, list):
        for i in list:
            tmp = os.path.normpath(i)
            if not os.path.isabs(tmp):
                tmp = os.path.abspath(tmp)
            list[list.index(i)] = tmp
        return list


    def parse_args(self, argv):
        parser = OptionParser()
        parser.add_option("-s", "--res", action="append", type="string", dest="res_dir_arr")
        parser.add_option("-d", "--dst", action="store", type="string", dest="dst_dir")
        parser.add_option("-k", "--encryptkey", dest="encryptkey",default="2dxLua")
        parser.add_option("-b", "--encryptsign", dest="encryptsign",default="XXTEA")
        (options, args) = parser.parse_args(argv)

        # print options.res_dir_arr
        # print options.dst_dir
        # print options.encryptkey
        # print options.encryptsign

        self._res_dir_arr = self.normalize_path_in_list(options.res_dir_arr)
        self._dst_dir = options.dst_dir
        self._encryptsign = options.encryptsign
        self._encryptkey = options.encryptkey
        self._res_files = {}



    def handle_all_res_files(self):
        for res_dir in self._res_dir_arr:
            for res_file in self._res_files[res_dir]:
                self._current_res_dir = res_dir
                dst_res_file = self.get_output_file_path(res_file)
                # dst_res_file = os.path.abspath(dst_res_file)
                shutil.copy(res_file, dst_res_file)

                # check need to encrypt
                if self.isFileExtValid(os.path.splitext(dst_res_file)[1]):
                    bytesFile = open(dst_res_file, "rb+")
                    encryBytes = encrypt(bytesFile.read(), self._encryptkey)
                    encryBytes = self._encryptsign + encryBytes
                    bytesFile.seek(0)
                    bytesFile.write(encryBytes)
                    bytesFile.close()
                else:
                    print 'current file is not encrypted : %s' % (dst_res_file)



    def deep_iterate_dir(self, rootDir):
        for lists in os.listdir(rootDir):
            path = os.path.join(rootDir, lists)
            if os.path.isdir(path):
                self.deep_iterate_dir(path)
            elif os.path.isfile(path):
                self._res_files[self._current_res_dir].append(path)


    def get_output_file_path(self, resfile):
        resc_filepath = ""
        relative_path = resfile[len(self._current_res_dir)+1:]
        resc_filepath = os.path.join(self._dst_dir, relative_path)

        dst_rootpath = os.path.split(resc_filepath)[0]
        try:
            # print "creating dir (%s)" % (dst_rootpath)
            os.makedirs(dst_rootpath)
        except OSError:
            if os.path.exists(dst_rootpath) == False:
                print("Error: cannot create folder in "+dst_rootpath)

        return resc_filepath


    def isFileExtValid(self, file_ext):
        for ext in ValidFileExt:
            if ext == file_ext:
                return True
        return False



if __name__ == "__main__":
    rs = ResEncrypt()
    rs.run()