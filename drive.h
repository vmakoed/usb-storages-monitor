#ifndef DRIVE_H
#define DRIVE_H

#define MAX_SIZE_OF_PRODUCT_NAME 100

class Drive
{
private:
    char mountPoint;
    char *product;

public:
    Drive(char mountPoint, char product[]);
    char getMountPoint() const;
    void setMountPoint(char value);
    char *getProduct() const;
    void setProduct(char *value);
};

#endif // DRIVE_H
