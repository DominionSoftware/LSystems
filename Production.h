#pragma once


#include <cstring>

class Production
{

public:

    Production()
    {

    }

    size_t leftContextLength()
    {
        return leftContext.length();
    }

    size_t predecessorLength()
    {
        return predecessor.length();
    }

    size_t rightContextLength()
    {
        return rightContext.length();
    }

    size_t successorLength()
    {
        return successor.length();
    }

    const char * getLeftContext()
    {
        return leftContext.c_str();
    }

    void setLeftContext(const char * s)
    {
        if (*s == '*')
        {
            return;
        }
        leftContext.assign(s);

    }

    const char * getRightContext()
    {
        return rightContext.c_str();
    }

    void setRightContext(const char * s)
    {
        if (*s == '*')
        {
            return;
        }
        rightContext.assign(s);

    }

    const char *  getPredecessor()
    {
        return predecessor.c_str();
    }

    void setPredecessor(const char * s)
    {
        if (*s == '*')
        {
            return;
        }
        predecessor.assign(s);

    }

    const char * getSuccessor()
    {
        return successor.c_str();
    }

    void setSuccessor(const char * s)
    {
        if (*s == '*')
        {
            return;
        }
        successor.assign(s);
    }

    bool predecessorDifferent(const char * s2)
    {
       const char * s1 = predecessor.c_str();

        while (*s1 != 0)
            if (*s2++ != *s1++)
            {
                return (true);
            }
        return (false);
    }

    bool rightContextDifferent(std::map<char,uint8_t> & ignoreMap,char *s2)
    {
        const char * s1 = rightContext.c_str();
        while (1)
        {
            if (*s1 == 0)
            {
                return (0);
            }
            if (ignoreMap[*s2])
            {
                s2++;
            }
            else if (*s2 == '[')
            {
                s2 = skipright(s2 + 1) + 1;
            }
            else if (*s1++ != *s2++)
            {
                return (1);
            }
        }
    }

    int leftContextDifferent(std::map<char,uint8_t> & ignoreMap, char *s2)
    {
        const char * s1 = leftContext.c_str();

        s1 = s1 + leftContextLength() - 1;

        while (1)
        {
            if (*s1 == 0)
            {
                return (false);
            }
            if (ignoreMap[*s2] || (*s2 == '['))
            {
                s2--;
            }
            else if (*s2 == ']')
            {
                s2 = skipleft(s2);
            }
            else {
                if (*s1-- != *s2--)
                {
                    return (true);
                }
            }
        }
    }
private:
    char * skipleft(char *s)
    {
        int level = 0;
        s--;
        while (*s != 0)
        {
            switch (*s)
            {
                case ']':
                    ++
                            level;
                    break;
                case '[':
                    if (level == 0)
                    {
                        return (--s);
                    }
                    else
                    {
                        --
                                level;
                    }
                    break;
                default:
                    break;
            }
            s--;
        }
        return (s);
    }

    char * skipright(char *s)
    {


        int level = 0;
        while (*s != 0)
        {
            switch (*s)
            {
                case '[':
                    ++level;
                    break;
                case ']':
                    if (level == 0)
                    {
                        return (s);
                    }
                    else
                    {
                        --level;
                    }
                    break;
                default:
                    break;
            }
            s++;
        }
        return (s);
    }

    std::string leftContext;
    std::string predecessor;

    std::string rightContext;
    std::string successor;



};