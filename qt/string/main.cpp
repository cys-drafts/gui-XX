
#include <QtCore>

QTextStream cout(stdout, QIODevice::WriteOnly);

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QString s1("paris");
    QString s2("london");
    QString s = s1 + " " + s2;
    cout << s << endl;

    QString s3("hello");
    QString s4("qt");
    QList<QString> list;
    list << s3 << s4;
    QListIterator<QString> iter(list);
    while (iter.hasNext()) {
        cout << iter.next();
        if (iter.hasNext()) {
            cout << " ";
        }
    }
    cout << "!" << endl;

    list.clear();
    list << s3 << s4;
    QString s5 = list.join("~");
    cout << s5 << endl;

    QStringList lst = s5.split("~", QString::SplitBehavior::SkipEmptyParts);
    cout << lst.length() << endl;
    QStringListIterator it(lst);
    while (it.hasNext()) {
        cout << it.next() << endl;
    }
}
