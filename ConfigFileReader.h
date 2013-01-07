#ifndef CONFIG_FILE_READER_H
#define CONFIG_FILE_READER_H

// initial offsets of robots from global co-ordinates
struct TransformInfo {
  TransformInfo(int xo, int yo, int to)
    : xOffset(xo), yOffset(yo), thetaOffset(to) { }
  int xOffset;
  int yOffset;
  int thetaOffset;
};

// information about each host which is a server running on a robot
struct HostInfo {
  HostInfo(const char *ipa, int loc, int lac, TransformInfo ti, int rf)
    : ip(ipa), locationColor(loc), laserColor(lac),
      transformInfo(ti), requestFreq(rf) { }
  const char *ip;
  int locationColor;
  int laserColor;
  TransformInfo transformInfo;
  int requestFreq;	// in millisecond
};

// Performs checking of the custom file type
class ConfigFileReader {
public:
  ConfigFileReader(int c, char **v, ArArgumentParser *parser)
    : myArgc(c), myArgv(v), myParser(parser), fileType(-1) { }
  void readHostsFile(vector<HostInfo> &hostsInfo);

  static const char *hostsArg;
  static const char *hostsFileHeader;
  static const char *infoFields[];
  static const size_t infoFieldTypes;
  static const int hostsFileTypes = 4;
  static const char myCommentChar = '#';
  static const char *myFieldSeparator;
  static const char *mySubFieldSeparator;

private:
  int myArgc;
  char **myArgv;
  ArArgumentParser *myParser;
  int fileType;

  int checkFileArg();
  ifstream *getFieldTypes(vector<size_t> &fieldTypes);
  void getValidLine(ifstream &inFile, string &buffer);
  void getFieldTypeIndices(const string &buffer,
      			   vector<size_t> &fieldTypes);
  size_t matchFieldIndex(const char *fieldName);
  void getIntSubFields(const string &s, vector<int> &subFields);
};



#endif
