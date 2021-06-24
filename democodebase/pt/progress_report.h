#ifndef PT_PROGRESS_REPORT_H
#define PT_PROGRESS_REPORT_H

namespace pt {

class progress_report
{
public:
    virtual void advance() = 0;

protected:
    ~progress_report() {}
};

} // namespace content

#endif
