#include "Polynomial.h"

void printPolynomial(Polynomial * head, int first_flag) {
    if (!head) {
        return;
    }
    if (head->coefficient == 0 && head->power == 0) {
        if (first_flag) {
            printf("0\n");
        }
        return;
    }
    int coef_flag = 0;
    if (head->coefficient) {
        if (!first_flag) {
            if (head->coefficient > 0) {
                printf(" + ");
            }
            else {
                printf(" - ");
                head->coefficient = -head->coefficient;
            }
        }
        if (head->coefficient != 1) {
            printf("%d", head->coefficient);
            coef_flag = 1;
        }
    }
    if (head->power && head->coefficient) {
        printf("x");
        if (head->power != 1) {
            printf("^%d", head->power);
        }
    }
    else if (!coef_flag) {
        printf("%d", head->coefficient);
    }
    printPolynomial(head->next, 0);
    if (first_flag) {
        printf("\n");
    }
}

void freePolynomial(Polynomial * head) {
    if (!head) {
        return;
    }
    freePolynomial(head->next);
    free(head);
}

status_codes push_forwardPolynomial(Polynomial ** head, int _coefficient, int _power) {
    Polynomial * tmp = (Polynomial *)malloc(sizeof(Polynomial));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->coefficient = _coefficient;
    tmp->power = _power;
    tmp->next = (*head);
    (*head) = tmp;
    return OK;
}

status_codes push_backPolynomial(Polynomial * head, int _coefficient, int _power) {
    while (head->next && head->next->power > _power) {
        head = head->next;
    }
    Polynomial * tmp = (Polynomial *)malloc(sizeof(Polynomial));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->coefficient = _coefficient;
    tmp->power = _power;
    if (!head->next) {
        tmp->next = NULL;
    }
    else {
        tmp->next = head->next;
    }
    head->next = tmp;
    return OK;
}

status_codes add(Polynomial ** result, Polynomial * first, Polynomial * second) {
    while (1) {
        if (first && second) {
            if (first->power == second->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, first->coefficient + second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient + second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                
                first = first->next;
                second = second->next;
            }
            else if (first->power > second->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                first = first->next;
            }
            else if (second->power > first->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                second = second->next;
            }
        }
        else if (!second && first) {
            if (!(*result)) {
                if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            first = first->next;
        }
        else if (!first && second) {
            if (!(*result)) {
                if (push_forwardPolynomial(result, second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            second = second->next;
        }
        else {
            return OK;
        }
    }
}

status_codes sub(Polynomial ** result, Polynomial * first, Polynomial * second) {
    while (1) {
        if (first && second) {
            if (first->power == second->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, first->coefficient - second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient - second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                
                first = first->next;
                second = second->next;
            }
            else if (first->power > second->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                first = first->next;
            }
            else if (second->power > first->power) {
                if (!(*result)) {
                    if (push_forwardPolynomial(result, -second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), -second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                second = second->next;
            }
        }
        else if (!second && first) {
            if (!(*result)) {
                if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            first = first->next;
        }
        else if (!first && second) {
            if (!(*result)) {
                if (push_forwardPolynomial(result, -second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), -second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            second = second->next;
        }
        else {
            return OK;
        }
    }
}

status_codes copyPolynomial(Polynomial ** dest, Polynomial * src) {
    if (!src) {
        return OK;
    }
    if (!(*dest)) {
        if (push_forwardPolynomial(dest, src->coefficient, src->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        if (push_backPolynomial((*dest), src->coefficient, src->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    copyPolynomial(dest, src->next);
    return OK;
}

status_codes mult(Polynomial ** result, Polynomial * first, Polynomial * second) {
    Polynomial * copy = NULL;
    if (copyPolynomial(&copy, second) == NO_MEMORY) {
        return NO_MEMORY;
    }
    Polynomial * tmp = NULL;
    Polynomial * result_copy = NULL;
    while (first) {
        tmp = NULL;
        while (copy) {
            if (!tmp) {
                if (push_forwardPolynomial(&tmp, first->coefficient * copy->coefficient, first->power + copy->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial(tmp, first->coefficient * copy->coefficient, first->power + copy->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            Polynomial * tmp2 = copy->next;
            free(copy);
            copy = tmp2;

        }
        result_copy = NULL;
        if (copyPolynomial(&result_copy, (*result)) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(*result);
        (*result) = NULL;
        if (add(result, result_copy, tmp) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(result_copy);
        freePolynomial(copy);
        freePolynomial(tmp);
        if (copyPolynomial(&copy, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        if (!first->next) {
            break;
        }
        first = first->next;
    }
    freePolynomial(copy);
    return OK;
}

void multconst(Polynomial * result, const int coef) {
    if (!result) {
        return;
    }
    result->coefficient *= coef;
    multconst(result->next, coef);
}

status_codes divmod(Polynomial ** div_result, Polynomial ** mod_result, Polynomial * first, Polynomial * second) {
    Polynomial * copy = NULL;
    if (copyPolynomial(&copy, first) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->power >= second->power) {
        int coefficient = copy->coefficient / second->coefficient;
        if (!coefficient) {
            (*mod_result) = copy;
            return OK;
        }
        Polynomial * to_mult = NULL;
        if (push_forwardPolynomial(&to_mult, coefficient, copy->power - second->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
        if (!(*div_result)) {
            if (push_forwardPolynomial(div_result, coefficient, copy->power - second->power) == NO_MEMORY) {
                return NO_MEMORY;
            }
        }
        else {
            if (push_backPolynomial((*div_result), coefficient, copy->power - second->power) == NO_MEMORY) {
                return NO_MEMORY;
            }
        }
        Polynomial * mult_result_second = NULL;
        if (mult(&mult_result_second, to_mult, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        Polynomial * first_copy = NULL;
        if (copyPolynomial(&first_copy, copy) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(copy);
        copy = NULL;
        if (sub(&copy, first_copy, mult_result_second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(to_mult);
        freePolynomial(mult_result_second);
        freePolynomial(first_copy);
        if (copy->coefficient == 0) {
            if (!copy->next) {
                (*mod_result) = copy;
                return OK;
            }
            Polynomial * tmp = copy->next;
            free(copy);
            copy = tmp;
        }
        
    }
    (*mod_result) = copy;
    return OK;
}


double binary_pow(double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = abs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return binary_pow(base, power - 1) * base;
    else {
        double tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

status_codes eval(int * result, Polynomial * poly, const double value) {
    if (!poly) {
        return OK;
    }
    (*result) += poly->coefficient * binary_pow(value, poly->power);
    eval(result, poly->next, value);
    return OK;
}

status_codes cmps(Polynomial ** result, Polynomial * first, Polynomial * second) {
    if (!first) {
        return OK;
    }
    Polynomial * second_base = NULL;
    if (copyPolynomial(&second_base, second) == NO_MEMORY) {
        return NO_MEMORY;
    }
    for (int i = 0; i < first->power - 1; i++) {
        Polynomial * tmp = NULL;
        if (mult(&tmp, second_base, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(second_base);
        second_base = NULL;
        if (copyPolynomial(&second_base, tmp) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(tmp);
    }
    multconst(second_base, first->coefficient);
    Polynomial * tmp2 = NULL;
    if (copyPolynomial(&tmp2, (*result)) == NO_MEMORY) {
        return NO_MEMORY;
    }
    freePolynomial(*result);
    (*result) = NULL;
    if (add(result, tmp2, second_base) == NO_MEMORY) {
        return NO_MEMORY;
    }
    freePolynomial(tmp2);
    freePolynomial(second_base);
    cmps(result, first->next, second);
    return OK;
}

status_codes diff(Polynomial * poly) {
    if (!poly) {
        return OK;
    }
    poly->coefficient *= poly->power;
    poly->power--;
    diff(poly->next);
}