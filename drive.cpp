#include "drive.h"
#include <string.h>

char Drive::getMountPoint() const
{
    return mountPoint;
}

void Drive::setMountPoint(char value)
{
    mountPoint = value;
}

char *Drive::getProduct() const
{
    return product;
}

void Drive::setProduct(char *value)
{
    product = value;
}

Drive::Drive(char mountPoint, char product[])
{
    this->product = new char[MAX_SIZE_OF_PRODUCT_NAME];

    this->mountPoint = mountPoint;
    strcpy(this->product, product);
}

