#include <iostream>
#include <math.h>
#include <string>

struct Transformer;
struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;
struct Expression
{
virtual ~Expression() { }
virtual double evaluate() const = 0;
virtual Expression* transform(Transformer* tr) const = 0;
virtual std::string print() const = 0;
};

struct Transformer //pattern Visitor
{
virtual ~Transformer() { }
virtual Expression* transformNumber(Number const*) = 0;
virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
virtual Expression* transformVariable(Variable const*) = 0;
};

struct Number : Expression
{
Number(double value) {
this->value_ = value;
}
double value() const {
return this->value_;
}

double evaluate() const {
return this->value_;
}
Expression* transform(Transformer* tr) const {
return tr->transformNumber(this);
}

std::string print() const {
return std::to_string(this->value_);
}
private:
double value_;
};

struct BinaryOperation : Expression
{
enum {
PLUS = '+',
MINUS = '-',
DIV = '/',
MUL = '*'
};
BinaryOperation(Expression const* left, int op, Expression const* right) {
this->left_ = left;
this->op_ = op;
this->right_ = right;
}
~BinaryOperation() {
delete left_;
delete right_;
}
double evaluate() const {
switch (this->op_)
{
case 43:
return this->left_->evaluate() + this->right_->evaluate();
case 45:
return this->left_->evaluate() - this->right_->evaluate();
case 47:
return this->left_->evaluate() / this->right_->evaluate();
case 42:
return this->left_->evaluate() * this->right_->evaluate();
default:
break;
}
}

Expression* transform(Transformer* tr) const {
return tr->transformBinaryOperation(this);
}

Expression const* left() const {
return this->left_;
}

Expression const* right() const {
return this->right_;
}
int operation() const {
return this->op_;
}

std::string print() const{
return this->left_->print() + std::string(1, this->op_) + this->right_->print();
}


private:
Expression const* left_;
Expression const* right_;
int op_;
};

struct FunctionCall : Expression
{

FunctionCall(std::string const& name, Expression const* arg) : name_(name), arg_(arg) {}
~FunctionCall() {
delete arg_;
}
double evaluate() const {
if (name_ == "sqrt") {
return sqrt(arg_->evaluate());
}
if (name_ == "abs") {
return abs(arg_->evaluate());
}
return this->arg_->evaluate();
}
Expression* transform(Transformer* tr) const {
return tr->transformFunctionCall(this);
}
std::string const& name() const {
return this->name_;
}
Expression const* arg() const {
return this->arg_;
}

std::string print() const {
return this->name_ +"(" + this->arg_->print() + ")";
}
private:
std::string const name_;
Expression const* arg_;
};

struct Variable : Expression
{
Variable(std::string const name) : name_(name) {}
std::string const& name() const {
return this->name_;
}
double evaluate() const {
return 0.0;
}
Expression* transform(Transformer* tr) const {
return tr->transformVariable(this);
}

std::string print() const {
return this->name_;
}

private:
std::string const name_;
};


struct CopySyntaxTree : Transformer
{
Expression* transformNumber(Number const* number)
{
return new Number(number->evaluate());
}
Expression* transformBinaryOperation(BinaryOperation const* binop)
{
return new BinaryOperation( binop->left()->transform(this), binop->operation(), binop->right()->transform(this));
}
Expression* transformFunctionCall(FunctionCall const* fcall)
{
return new FunctionCall(fcall->name(), fcall->arg()->transform(this));
}
Expression* transformVariable(Variable const* var)
{
return new Variable(var->name());
}
};


struct FoldConstants : Transformer
{
Expression* transformNumber(Number const* number)
{
return new Number(number->evaluate());
 
}
Expression* transformBinaryOperation(BinaryOperation const* binop)
{
Expression* left = binop->left()->transform(this);
Expression* right = binop->right()->transform(this);
Number* left_ = dynamic_cast <Number*>(left);
Number* right_ = dynamic_cast <Number*>(right);
if (left_ && right_){
Expression* newBinOp = new BinaryOperation(left, binop->operation(), right);
double value = newBinOp->evaluate();
delete newBinOp;
return new Number(value);
}
return new BinaryOperation(left, binop->operation(), right);
}

Expression* transformFunctionCall(FunctionCall const* fcall)
{
Expression* arg = fcall->arg()->transform(this);
Number* number = dynamic_cast <Number*>(arg);
if (number) {
Expression* fnewCall = new FunctionCall(fcall->name(), number);
double tmp = fnewCall->evaluate();
delete fnewCall;
return new Number(tmp);
}
return new FunctionCall(fcall->name(), arg);
}
Expression* transformVariable(Variable const* var)
{
return new Variable(var->name());
}

};


int main()
{
Number* n32 = new Number(32.0);
Number* n16 = new Number(16.0);
BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
Variable* var = new Variable("var");
BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
FunctionCall* callAbs = new FunctionCall("abs", mult);
CopySyntaxTree CST;
Expression* newExpr = callAbs->transform(&CST);
std::cout << newExpr->print()<< std::endl;
FoldConstants FC;
Expression* newExpr1 = callAbs->transform(&FC);
std::cout << newExpr1->print();
}
