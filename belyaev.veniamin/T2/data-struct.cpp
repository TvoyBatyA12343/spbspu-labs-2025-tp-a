#include "data-struct.hpp"
#include "io-helpers.hpp"
#include "stream-guard.hpp"

bool belyaev::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }

  return lhs.key3.size() < rhs.key3.size();
}

std::istream& belyaev::operator>>(std::istream& in, DataStruct& dst)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct newDS;
  bool hasKeys[] = {false, false, false};
  in >> DelimeterIO{'('};

  for (int i = 0; i < 3; i++)
  {
    in >> DelimeterIO{':'};
    in >> DelimeterIO{'k'};
    in >> DelimeterIO{'e'};
    in >> DelimeterIO{'y'};

    int keyNum = 0;
    in >> keyNum;
    if (keyNum == 1 && !hasKeys[0])
    {
      in >> DoubleEIOIn{newDS.key1};
      hasKeys[0] = true;
    }
    else if (keyNum == 2 && !hasKeys[1])
    {
      in >> PairLLIOIn{newDS.key2};
      hasKeys[1] = true;
    }
    else if (keyNum == 3 && !hasKeys[2])
    {
      in >> StringIO{newDS.key3};
      hasKeys[2] = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimeterIO{':'};
  in >> DelimeterIO{')'};

  if (!(hasKeys[0] && hasKeys[1] && hasKeys[2]))
  {
    in.setstate(std::ios::failbit);
  }

  if (in)
  {
    dst = newDS;
  }
  return in;
}

std::ostream& belyaev::operator<<(std::ostream& out, const DataStruct& dst)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << DoubleEIOOut{dst.key1};
  out << ":key2 " << PairLLIOOut{dst.key2};
  out << ":key3 \"" << dst.key3 << "\"";
  out << ":)";
  return out;
}
