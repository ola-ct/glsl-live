#include "glslhighlighter.h"

GLSLHighlighter::GLSLHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , m_markCaseSensitivity(Qt::CaseInsensitive)
{
    // default color scheme
    m_colors[GLSLEdit::Normal]     = QColor("#000000");
    m_colors[GLSLEdit::Comment]    = QColor("#808080");
    m_colors[GLSLEdit::Number]     = QColor("#008000");
    m_colors[GLSLEdit::String]     = QColor("#f00000");
    m_colors[GLSLEdit::Operator]   = QColor("#808000");
    m_colors[GLSLEdit::Identifier] = QColor("#000020");
    m_colors[GLSLEdit::Keyword]    = QColor("#000080");
    m_colors[GLSLEdit::BuiltIn]    = QColor("#00c0a0");
    m_colors[GLSLEdit::Marker]     = QColor("#ffff00");

    m_keywords << "for";
    m_keywords << "break";
    m_keywords << "continue";
    m_keywords << "do";
    m_keywords << "while";
    m_keywords << "if";
    m_keywords << "else";
    m_keywords << "return";
    m_keywords << "discard";

    m_keywords << "true";
    m_keywords << "false";

    m_knownIds << "radians";
    m_knownIds << "degrees";
    m_knownIds << "sin";
    m_knownIds << "cos";
    m_knownIds << "tan";
    m_knownIds << "asin";
    m_knownIds << "acos";
    m_knownIds << "atan";
    m_knownIds << "pow";
    m_knownIds << "exp";
    m_knownIds << "log";
    m_knownIds << "log2";
    m_knownIds << "sqrt";
    m_knownIds << "inversesqrt";

    m_knownIds << "abs";
    m_knownIds << "sign";
    m_knownIds << "floor";
    m_knownIds << "ceil";
    m_knownIds << "fract";
    m_knownIds << "mod";
    m_knownIds << "min";
    m_knownIds << "max";
    m_knownIds << "clamp";
    m_knownIds << "mix";
    m_knownIds << "step";
    m_knownIds << "smoothstep";

    m_knownIds << "length";
    m_knownIds << "distance";
    m_knownIds << "dot";
    m_knownIds << "cross";
    m_knownIds << "normalize";
    m_knownIds << "faceforward";
    m_knownIds << "reflect";
    m_knownIds << "refract";

    m_knownIds << "matrixCompMult";

    m_knownIds << "lessThan";
    m_knownIds << "lessThanEqual";
    m_knownIds << "greaterThan";
    m_knownIds << "greaterThanEqual";
    m_knownIds << "equal";
    m_knownIds << "notEqual";
    m_knownIds << "any";
    m_knownIds << "all";
    m_knownIds << "not";

    m_knownIds << "texture2DLod";
    m_knownIds << "texture2DProjLod";
    m_knownIds << "texture2DCubeLod";
    m_knownIds << "texture2D";
    m_knownIds << "texture2DProj";
    m_knownIds << "textureCube";

    m_knownIds << "gl_Position";
    m_knownIds << "gl_FragCoord";
    m_knownIds << "gl_PointSize";
    m_knownIds << "gl_FragColor";
    m_knownIds << "gl_FragData";
    m_knownIds << "gl_FrontFacing";

    m_keywords << "attribute";
    m_keywords << "uniform";
    m_keywords << "varying";
    m_keywords << "const";
    m_keywords << "lowp";
    m_keywords << "mediump";
    m_keywords << "highp";

    m_keywords << "in";
    m_keywords << "out";
    m_keywords << "inout";

    m_knownIds << "bvec2";
    m_knownIds << "bvec3";
    m_knownIds << "bvec4";
    m_knownIds << "ivec2";
    m_knownIds << "ivec3";
    m_knownIds << "ivec4";
    m_knownIds << "vec2";
    m_knownIds << "vec3";
    m_knownIds << "vec4";
    m_knownIds << "mat2";
    m_knownIds << "mat3";
    m_knownIds << "mat4";
    m_knownIds << "void";
    m_knownIds << "float";
    m_knownIds << "int";
    m_knownIds << "bool";
    m_knownIds << "sampler2D";
    m_knownIds << "samplerCube";
    m_knownIds << "struct";

}

void GLSLHighlighter::setColor(GLSLEdit::ColorComponent component, const QColor& color)
{
    m_colors[component] = color;
    rehighlight();
}

void GLSLHighlighter::highlightBlock(const QString& text)
{
    // parsing state
    enum {
        Start,
        Number,
        Identifier,
        String,
        Comment
    };

    QList<int> bracketPositions;

    int blockState = previousBlockState();
    int bracketLevel = blockState >> 4;
    int state = blockState & 15;
    if (blockState < 0) {
        bracketLevel = 0;
        state = Start;
    }

    int start = 0;
    int i = 0;
    while (i <= text.length()) {
        QChar ch = (i < text.length()) ? text.at(i) : QChar();
        QChar next = (i < text.length() - 1) ? text.at(i + 1) : QChar();

        switch (state) {

        case Start:
            start = i;
            if (ch.isSpace()) {
                ++i;
            } else if (ch.isDigit()) {
                ++i;
                state = Number;
            } else if (ch.isLetter() || ch == '_') {
                ++i;
                state = Identifier;
            } else if (ch == '\'' || ch == '\"') {
                ++i;
                state = String;
            } else if (ch == '/' && next == '*') {
                ++i;
                ++i;
                state = Comment;
            } else if (ch == '/' && next == '/') {
                i = text.length();
                setFormat(start, text.length(), m_colors[GLSLEdit::Comment]);
            } else {
                if (!QString("(){}[]").contains(ch))
                    setFormat(start, 1, m_colors[GLSLEdit::Operator]);
                if (ch =='{' || ch == '}') {
                    bracketPositions += i;
                    if (ch == '{')
                        bracketLevel++;
                    else
                        bracketLevel--;
                }
                ++i;
                state = Start;
            }
            break;

        case Number:
            if (ch.isSpace() || !ch.isDigit()) {
                setFormat(start, i - start, m_colors[GLSLEdit::Number]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case Identifier:
            if (ch.isSpace() || !(ch.isDigit() || ch.isLetter() || ch == '_')) {
                QString token = text.mid(start, i - start).trimmed();
                if (m_keywords.contains(token))
                    setFormat(start, i - start, m_colors[GLSLEdit::Keyword]);
                else if (m_knownIds.contains(token))
                    setFormat(start, i - start, m_colors[GLSLEdit::BuiltIn]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case String:
            if (ch == text.at(start)) {
                QChar prev = (i > 0) ? text.at(i - 1) : QChar();
                if (prev != '\\') {
                    ++i;
                    setFormat(start, i - start, m_colors[GLSLEdit::String]);
                    state = Start;
                } else {
                    ++i;
                }
            } else {
                ++i;
            }
            break;

        case Comment:
            if (ch == '*' && next == '/') {
                ++i;
                ++i;
                setFormat(start, i - start, m_colors[GLSLEdit::Comment]);
                state = Start;
            } else {
                ++i;
            }
            break;

        default:
            state = Start;
            break;
        }
    }

    if (state == Comment)
        setFormat(start, text.length(), m_colors[GLSLEdit::Comment]);
    else
        state = Start;

    if (!m_markString.isEmpty()) {
        int pos = 0;
        int len = m_markString.length();
        QTextCharFormat markerFormat;
        markerFormat.setBackground(m_colors[GLSLEdit::Marker]);
        markerFormat.setForeground(m_colors[GLSLEdit::Normal]);
        for (;;) {
            pos = text.indexOf(m_markString, pos, m_markCaseSensitivity);
            if (pos < 0)
                break;
            setFormat(pos, len, markerFormat);
            ++pos;
        }
    }

    if (!bracketPositions.isEmpty()) {
        GLSLBlockData *blockData = reinterpret_cast<GLSLBlockData*>(currentBlock().userData());
        if (!blockData) {
            blockData = new GLSLBlockData;
            currentBlock().setUserData(blockData);
        }
        blockData->bracketPositions = bracketPositions;
    }

    blockState = (state & 15) | (bracketLevel << 4);
    setCurrentBlockState(blockState);
}

void GLSLHighlighter::mark(const QString &str, Qt::CaseSensitivity caseSensitivity)
{
    m_markString = str;
    m_markCaseSensitivity = caseSensitivity;
    rehighlight();
}

QStringList GLSLHighlighter::keywords(void) const
{
    return m_keywords.toList();
}

void GLSLHighlighter::setKeywords(const QStringList& keywords)
{
    m_keywords = QSet<QString>::fromList(keywords);
    rehighlight();
}

