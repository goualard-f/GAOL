%% manual.cls
%% Copyright (c) 2006 Frederic Goualard
%%                    Laboratoire d'Informatique de Nantes-Atlantique
\def\MAN@version{2010/04/26 v2.0}

\NeedsTeXFormat{LaTeX2e}[1994/06/01]

\ProvidesClass{manual}[\MAN@version]

\LoadClass{report}

\RequirePackage{wrapfig}
\RequirePackage[T1]{fontenc}
\RequirePackage{textcomp}

%\RequirePackage[document]{ragged2e}

\RequirePackage{multicol}

%\RequirePackage[scaled=.92]{helvet}
%\renewcommand{\familydefault}{\sfdefault}
%\RequirePackage{times}
%\RequirePackage{mathpazo}
%\RequirePackage{fourier}
%\RequirePackage{calligra}
%\RequirePackage{avant}
%\RequirePackage{chancery}
%\RequirePackage{bookman}
%\RequirePackage{newcent}
%\RequirePackage{charter}
%\RequirePackage{pandora}

\RequirePackage{marvosym}

\RequirePackage{listings}
\lstset{defaultdialect=C++}
%\RequirePackage[a4paper,text={15cm,24cm},centering]{geometry}
\RequirePackage{fancyvrb}

\RequirePackage{color}

\RequirePackage[a4paper,textheight=23cm,left=2cm,marginparwidth=6cm,marginparsep=.4cm,right=.7cm,includemp=true,twoside]{geometry}
\RequirePackage[citeonce(chapter)]{marginbib}

\setlength{\wrapoverhang}{\marginparwidth}
\addtolength{\wrapoverhang}{\marginparsep}

\RequirePackage{index}

%% !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
%% This package should come last in the list of "\RequirePackage"
%% !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
\RequirePackage[breaklinks=true,colorlinks=true]{hyperref}
\RequirePackage{nameref}

%% Information in the margin
\newcommand*{\important}{\marginpar[~\hfill\textcolor{red}{\huge\Stopsign}]{\textcolor{red}{\huge\Stopsign}}}


\newtheorem{definition}{Definition}

%% Index of methods, functions, vars...
%\newindex{fun}{fux}{fnd}{Programs, functions, methods, constants and variables index}
\newcommand*{\findex}[1]{\index[idx]{#1@\texttt{#1}}}
%% Index of concepts
\newindex{idx}{idx}{ind}{Index}
\newcommand*{\cindex}[1]{\index[idx]{#1}}
\newcommand*{\idxself}{\~{}}
\newcommand*{\hyperpagebf}[1]{\textbf{\hyperpage{#1}}}
% Principal entry in the index (page number in bold)
\newcommand*{\defindex}[1]{\index[idx]{#1@\texttt{#1}|textbf}}

\definecolor{lightblue}{rgb}{0.6,0.6,1}

\newcommand{\mainmatter}{\cleardoublepage\pagenumbering{arabic}\setcounter{page}{1}}

\renewcommand\maketitle{\begin{titlepage}%
    \hoffset=4.5cm
    \let\footnotesize\small
    \let\footnoterule\relax
    \let \footnote \thanks
    \null
    \vskip 3.5cm
      \noindent\rule{\textwidth}{1mm}
      \vskip4em
      \noindent{\usefont{T1}{phv}{b}{n}\fontsize{50pt}{54pt}\selectfont\@title \par}%
      \vskip 1em%
      \noindent{\usefont{T1}{phv}{b}{it}\fontsize{18pt}{20pt}\selectfont\MAN@subtitle \par}%
      \vskip4em\par
      \noindent\rule{\textwidth}{1mm}\par
      \vspace{-1em}
      \begin{flushright}\usefont{T1}{phv}{m}{n}\fontsize{10pt}{10pt}\selectfont
      Edition \edition\par
      Last updated \updated
      \end{flushright}
      
      \vskip 4cm
      \noindent{\usefont{T1}{phv}{b}{n}\fontsize{24pt}{24pt}\selectfont\@author \par}
      \medskip
      \noindent{\usefont{T1}{phv}{m}{n}\fontsize{12pt}{12pt}\selectfont\MAN@institute \par}
  \end{titlepage}%
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}

\newcommand{\schapter}[1]{%
  \chapter*{#1}
  \addcontentsline{toc}{chapter}{#1}%
}


\newcommand{\subtitle}[1]{\gdef\MAN@subtitle{#1}}
\newcommand{\institute}[1]{\gdef\MAN@institute{#1}}

\newcommand*{\code}[1]{\texttt{#1}}
\newcommand*{\element}[1]{\textcolor{blue}{{\textbf{\code{#1}}}}}

\newcommand{\outputs}[1]{\textcolor{red}{\textsl{\Printer{} #1}}}

\newcommand*{\pxref}[1]{see Section~\hyperref[#1]{\ref{#1}}, page~\pageref{#1}}
\newcommand*{\spxref}[1]{see Section~\hyperref[#1]{[\nameref{#1}]}, page~\pageref{#1}}
\newcommand*{\xref}[1]{See Section~\hyperref[#1]{\ref{#1}}, page~\pageref{#1}}
\newcommand*{\sxref}[1]{See Section~\hyperref[#1]{[\nameref{#1}]}, page~\pageref{#1}}
\newcommand*{\exref}[1]{Section~\hyperref[#1]{\ref{#1}}, page~\pageref{#1}}
\newcommand*{\sexref}[1]{Section~\hyperref[#1]{[\nameref{#1}]}, page~\pageref{#1}}

\newcommand{\RSet}{\ensuremath{\mathbb{R}}}
\newcommand{\FSet}{\ensuremath{\mathbb{F}}}

%% Definition
\newcommand{\dfn}[1]{\textsl{#1}}

%% Name of a command/program
\newcommand{\cmd}[1]{\texttt{#1}}

%% Name of an option
\newcommand{\option}[1]{\texttt{#1}}
%% Name of a long option
\newcommand{\loption}[1]{\texttt{-{}-#1}}

%% Name of a variable
\newcommand{\var}[1]{\textsl{#1}}

%% Name of a file
\newcommand{\file}[1]{\texttt{#1}}

%% Name of an environment variable
\newcommand{\env}[1]{\texttt{#1}\index[idx]{#1@\texttt{#1}}}

%% Output of a program
\DefineVerbatimEnvironment{onscreen}{Verbatim}{commandchars=\@\^\~,frame=single,label={\textsf{Output}}}

%% Example program
\def\example{\medskip\examplevb
}
\def\endexample{\endexamplevb
}
\DefineVerbatimEnvironment{examplevb}{Verbatim}{fontsize=\small,commandchars=\@\^\~,frame=single,label={\textsf{Example}},numbers=left,numbersep=2pt}

%% Command
\DefineVerbatimEnvironment{cmdshell}{Verbatim}{commandchars=\@\^\~}


\let\@firsttime=\@undefined

%% Definition of function
%%  #1=return type
%%  #2=name
%%  #3=parameters
\newcommand{\deffunx}[3]{%
  \defindex{#2}%
  \ifx\@firsttime\@undefined\medskip\fi
  \gdef\@firsttime{}
  \noindent\textcolor{blue}{\textsf{#1 \textbf{#2} #3}}\par
}
\newenvironment{deffun}[3]{%
  \deffunx{#1}{#2}{#3}
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
  \global\let\@firsttime=\@undefined
}

%% Definition of a template of a function
%%  #1=template (e.g., "template <typename T>")
%%  #2=return type
%%  #3=name
%%  #4=parameters
\newcommand{\defTfunx}[4]{%
  \defindex{#3}%
  \ifx\@firsttime\@undefined\medskip\fi
  \gdef\@firsttime{}
  \noindent\bgroup\color{blue}\textsf{#1}\par%
  \noindent\textsf{#2 \textbf{#3} #4}\egroup\par}
\newenvironment{defTfun}[4]{%
  \defTfunx{#1}{#2}{#3}{#4}
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
  \global\let\@firsttime=\@undefined
}


%% Definition of a method
%%  #1=class
%%  #2=return type
%%  #3=name
%%  #4=parameters
\newcommand{\defmethodx}[4]{%
  \defindex{#3}%
  \ifx\@firsttime\@undefined\medskip\fi
  \gdef\@firsttime{}
  \noindent\textcolor{blue}{\textsf{\if#2\@empty\else #2 \fi#1::\textbf{#3} #4}}\par
}
\newenvironment{defmethod}[4]{%
  \defmethodx{#1}{#2}{#3}{#4}
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
  \global\let\@firsttime=\@undefined
}

%% Definition of a static method
%%  #1=class
%%  #2=return type
%%  #3=name
%%  #4=parameters
\newcommand{\defstaticmethodx}[4]{%
  \defindex{#3}%
  \ifx\@firsttime\@undefined\medskip\fi
  \gdef\@firsttime{}
  \noindent\textcolor{blue}{\sffamily#2 #1::{\bfseries#3} #4\hfill static}\par
}
\newenvironment{defstaticmethod}[4]{%
  \defstaticmethodx{#1}{#2}{#3}{#4}
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
  \global\let\@firsttime=\@undefined
}

%% Definition of a type
\newenvironment{deftype}[2]{%
  \defindex{#2}%
  \noindent\textcolor{blue}{\textsf{\textbf{#2}}\hfill #1}\par
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
}

%% Definition of a constant
\newenvironment{defconst}[2]{%
  \defindex{#2}%
  \medskip
  \noindent\textcolor{blue}{\textsf{#1 \textbf{#2}}\hfill const}\par
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
}

%% Definition of a macro without parameter
\newenvironment{defmacro}[1]{%
  \defindex{#1}%
  \medskip
  \noindent\textcolor{blue}{\textsf{\textbf{#1}}\hfill macro}\par
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
}

%% Definition of a macro with parameters
%% #1=name of the macro
%% #2=parameters
\newenvironment{defmacroA}[2]{%
  \defindex{#1}%
  \medskip
  \noindent\textcolor{blue}{\textsf{\textbf{#1} #2}\hfill macro}\par
  \begin{flushright}
  \begin{minipage}{.94\textwidth}
}{%
  \end{minipage}
  \end{flushright}
  \medskip
}

\newenvironment{operation}{%
  \vspace*{-5pt}
  \def\op##1{%
    \noindent\color{lightblue}\ensuremath{\blacksquare} ##1\par
  }
}{%
  \par\smallskip
}


\newenvironment{copyrighttxt}{%
  \parindent0pt%
  \newpage
  \thispagestyle{empty}%
  \null\vfill
}{%
  \newpage
}


%% Comment
\newcommand{\rem}[1]{\textcolor{red}{\textsl{#1}}}

\def\cleardoublepage{\clearpage%\thispagestyle{empty}%
  \ifodd\c@page\else
    \hbox{}\thispagestyle{empty}\newpage\if@twocolumn\hbox{}\newpage\fi\fi}

\renewcommand\chapter{\cleardoublepage
                    \thispagestyle{plain}%
                    \global\@topnum\z@
                    \@afterindentfalse
                    \secdef\@chapter\@schapter}
\def\@makechapterhead#1{%
  \vspace*{50\p@}%
  {\parindent \z@ \raggedright \normalfont
    \begin{tabular}{r|}
      \hbox{ }\\
    \ifnum \c@secnumdepth >\m@ne
      \usefont{T1}{phv}{m}{b}\fontsize{80pt}{50pt}\selectfont\thechapter\\[20pt]
    \fi
    \begin{minipage}{.96\textwidth}
      \begin{flushright}
      \usefont{T1}{phv}{m}{b}\fontsize{25pt}{25pt}\selectfont #1
      \end{flushright}
    \end{minipage}
    \end{tabular}
    \vskip 40\p@
  }}

\def\@makeschapterhead#1{%
  \cleardoublepage
  \vspace*{50\p@}%
  {\parindent \z@ \raggedright
    \normalfont
    \begin{tabular}{r|}
      \hbox{ }\\
      \begin{minipage}{.96\textwidth}
      \begin{flushright}
      \usefont{T1}{phv}{m}{b}\fontsize{25pt}{25pt}\selectfont #1
      \end{flushright}
    \end{minipage}\\[10pt]
    \end{tabular}
    \vskip 40\p@
  }}

\def\ps@plain{\let\@mkboth\@gobbletwo
     \let\@oddhead\@empty%
      % 9.15cm is the middle of the printed area
     \def\@oddfoot{\reset@font\hspace*{9.15cm}\thepage}%
     \let\@evenhead\@empty%
     \def\@evenfoot{\reset@font\hspace*{2.75cm}\thepage}}

\pagenumbering{roman}
\pagestyle{plain}
\endinput
%% END OF FILE
