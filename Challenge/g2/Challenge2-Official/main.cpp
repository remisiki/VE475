#include <fstream>
#include <getopt.h>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

using std::cout, std::endl, std::string;
using sstream = std::stringstream;

static const string CHALLENGE_CIPHERTEXT = "Zlove;VEHFO";
static const string allchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.;?!()";

char rotate(char original, int key)
{
  size_t loc = allchars.find(original);
  if (loc == string::npos) // not found
    throw std::runtime_error("invalid character");
  return allchars[(69 + ((int)loc + key) % 69) % 69];
}

string caesar(string m, int key)
{
  string c;
  for (char ch : m)
    c.push_back(rotate(ch, key));
  return c;
}

long generate(bool use_default = false)
{
  if (use_default)
    return 114514;
  std::random_device rd;
  std::mt19937 gen(rd());
  return gen();
}

enum Operation {
  ENCRYPT,
  DECRYPT,
  GENERATE,
  NONE
};

int main(int argc, char* argv[])
{
  int opt;
  string message, keyloc = "";
  Operation operation = NONE;
  auto key = generate(true);
  static option opts[] = { { "generate", no_argument, NULL, 'g' },
    { "decrypt", required_argument, NULL, 'd' },
    { "encrypt", required_argument, NULL, 'e' },
    { "key", required_argument, NULL, 'k' },
    { NULL, 0, NULL, 0 } };
  while ((opt = getopt_long(argc, argv, ":gd:e:k:", opts, NULL)) != -1)
    switch (opt) {
    case 'g': {
      if (operation == NONE)
        operation = GENERATE;
      break;
    }
    case 'd': {
      if (operation == NONE)
      {
        operation = DECRYPT;
        message = optarg;
      }
      break;
    }
    case 'e': {
      if (operation == NONE)
      {
        operation = ENCRYPT;
        message = optarg;
      }
      break;
    }
    case 'k': {
      keyloc = optarg;
      break;
    }
    }
  if (keyloc != "") {
    std::fstream keyfile(keyloc);
    keyfile >> key;
    char c;
    keyfile.getline(&c, 1);
    if (keyfile.fail() || c != '\0')
      throw std::runtime_error("invalid key");
    keyfile.close();
  }
  switch (operation) {
  case GENERATE: {
    cout << generate() << endl;
    break;
  }
  case DECRYPT: {
    if (message == CHALLENGE_CIPHERTEXT && keyloc == "")
      cout << "cheater: it is forbidden to decrypt the challenge ciphertext" << endl;
    else
      cout << caesar(message, -key) << endl;
    break;
  }
  case ENCRYPT: {
    cout << caesar(message, key) << endl;
    break;
  }
  case NONE: {
    cout << "there is nothing to do" << endl;
    break;
  }
  }
  return 0;
}
